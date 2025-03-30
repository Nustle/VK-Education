import numpy as np
import math


class SoftmaxRegression:
    def __init__(
            self,
            *,
            penalty="l2",
            alpha=0.0001,
            max_iter=100,
            tol=0.001,
            random_state=None,
            eta0=0.01,
            early_stopping=False,
            validation_fraction=0.1,
            n_iter_no_change=5,
            shuffle=True,
            batch_size=32
    ):
        self.penalty = penalty
        self.alpha = alpha
        self.max_iter = max_iter
        self.tol = tol
        self.random_state = random_state
        self.eta0 = eta0
        self.early_stopping = early_stopping
        self.validation_fraction = validation_fraction
        self.n_iter_no_change = n_iter_no_change
        self.shuffle = shuffle
        self.batch_size = batch_size
        self.npr = np.random.RandomState(self.random_state)
        self.w = None
        self.bias = None

    @staticmethod
    def one_hot_encoding(y):
        classes = y.max() + 1
        y_one_hot = np.eye(classes, dtype=int)
        return y_one_hot[y]

    def log_loss(self, x, y):
        n = x.shape[0]
        loss1 = -np.sum(y * (x @ self.w + self.bias))
        exp = np.exp(x @ self.w + self.bias)
        sum_exp = exp.sum(axis=-1, keepdims=True)
        loss2 = np.sum(np.log(sum_exp))
        return (loss1 + loss2) / n

    def log_loss_grad(self, x, y, batch):
        n = batch.shape[0]
        proba = self.softmax(x[batch, :] @ self.w + self.bias)
        return x[batch, :].T @ (proba - y[batch, :]) / n

    def log_loss_grad_bias(self, x, y, batch):
        n = batch.shape[0]
        proba = self.softmax(x[batch, :] @ self.w + self.bias)
        y_one_hot_class = y[batch, :].T.sum(axis=-1)
        proba_class = proba.T.sum(axis=-1)
        return (proba_class - y_one_hot_class) / n

    def get_penalty_grad(self):
        if self.penalty == "l1":
            return self.alpha * np.sign(self.w)
        elif self.penalty == "l2":
            return self.alpha * 2 * self.w

    def get_batches(self, x):
        n = x.shape[0]
        count_batches = n // self.batch_size
        x_ind = np.arange(0, n)
        if self.shuffle:
            x_ind = self.npr.permutation(x_ind)
        batches = x_ind[:count_batches * self.batch_size]
        batch_first = x_ind[np.isin(x_ind, batches) is False]
        return batch_first, batches.reshape(count_batches, self.batch_size)

    def fit(self, x, y):
        y = self.one_hot_encoding(y)

        self.w = self.npr.uniform(-2, 2, size=(x.shape[1], y.shape[1]))
        self.bias = self.npr.uniform(-0.3, 0.3, size=y.shape[1])

        x_train, y_train = x, y
        x_test, y_test = np.array([]), np.array([])
        if self.early_stopping:
            valid = math.ceil(x.shape[0] * self.validation_fraction)
            x_train, y_train = x[valid:], y[valid:]
            x_test, y_test = x[:valid], y[:valid]

        loss = self.log_loss(x_train, y_train)

        best_loss = np.inf
        best_loss_test = self.log_loss(x_test, y_test) if self.early_stopping else 0

        current_iter = 1
        valid_no_change_iter = 0

        while current_iter < self.max_iter\
                and loss <= best_loss - self.tol\
                and not (self.early_stopping and valid_no_change_iter == self.n_iter_no_change):

            best_loss = loss
            batch_first, batches = self.get_batches(x_train)

            if batch_first.shape[0] > 0:
                grad = self.log_loss_grad(x_train, y_train, batch_first) + self.get_penalty_grad()
                w_new = self.w - self.eta0 * grad
                self.w = w_new
                bias_new = self.bias - self.eta0 * self.log_loss_grad_bias(x_train, y_train, batch_first)
                self.bias = bias_new

            for j in range(batches.shape[0]):
                grad = self.log_loss_grad(x_train, y_train, batches[j]) + self.get_penalty_grad()
                w_new = self.w - self.eta0 * grad
                self.w = w_new
                bias_new = self.bias - self.eta0 * self.log_loss_grad_bias(x_train, y_train, batches[j])
                self.bias = bias_new

            if self.early_stopping:
                loss_test = self.log_loss(x_test, y_test)
                if loss_test > best_loss_test - self.tol:
                    valid_no_change_iter += 1
                else:
                    best_loss_test = loss_test
                    valid_no_change_iter = 0

            loss = self.log_loss(x_train, y_train)
            current_iter += 1

    def predict_proba(self, x):
        proba = self.softmax(x @ self.w + self.bias)
        return proba

    def predict(self, x):
        proba = self.softmax(x @ self.w + self.bias)
        return proba.argmax(axis=-1, keepdims=True)

    @staticmethod
    def softmax(z):
        """
        Calculates a softmax normalization over the last axis

        Examples:

        >>> softmax(np.array([1, 2, 3]))
        [0.09003057 0.24472847 0.66524096]

        >>> softmax(np.array([[1, 2, 3], [4, 5, 6]]))
        [[0.09003057 0.24472847 0.66524096]
         [0.09003057 0.24472847 0.66524096]]
        :param z: np.array, size: (d0, d1, ..., dn)
        :return: np.array of the same size as z
        """
        max_vals = z.max(axis=-1, keepdims=True)
        z = np.exp(z - max_vals)
        sum_vals = z.sum(axis=-1, keepdims=True)
        return z / sum_vals

    @property
    def coef_(self):
        return self.w

    @property
    def intercept_(self):
        return self.bias

    @coef_.setter
    def coef_(self, value):
        self.w = value

    @intercept_.setter
    def intercept_(self, value):
        self.bias = value

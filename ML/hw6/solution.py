import numpy as np
from sklearn.tree import DecisionTreeRegressor


class GBCustomRegressor:
    def __init__(
            self,
            *,
            learning_rate=0.1,
            n_estimators=100,
            criterion="friedman_mse",
            min_samples_split=2,
            min_samples_leaf=1,
            max_depth=3,
            random_state=None
    ):
        self.learning_rate = learning_rate
        self.n_estimators = n_estimators
        self.criterion = criterion
        self.min_samples_split = min_samples_split
        self.min_samples_leaf = min_samples_leaf
        self.max_depth = max_depth
        self.random_state = random_state
        self.f0 = None
        self.coef = []
        self.trees = []

    def get_loss(self, x, y):
        y_pred = self.predict(x)
        return (y_pred - y).T @ (y_pred - y)

    def get_loss_grad(self, x, y):
        y_pred = self.predict(x)
        return -(y_pred - y)

    def fit(self, x, y):
        self.f0 = y.mean()
        for t in range(self.n_estimators):
            pseudo_remainder = self.get_loss_grad(x, y)
            tree = DecisionTreeRegressor(criterion=self.criterion, min_samples_split=self.min_samples_split,
                                         min_samples_leaf=self.min_samples_leaf, max_depth=self.max_depth,
                                         random_state=self.random_state).fit(x, pseudo_remainder)
            tree_pred = tree.predict(x)
            rho = (pseudo_remainder.T @ tree_pred) / (tree_pred.T @ tree_pred)
            self.coef.append(self.learning_rate * rho)
            self.trees.append(tree)

    def predict(self, x):
        y_pred = self.f0
        if len(self.trees) != 0:
            trees_pred = np.vstack([tree.predict(x) for tree in self.trees]).T
            y_pred += trees_pred @ np.array(self.coef)
        return y_pred

    @property
    def estimators_(self):
        return self.trees


class GBCustomClassifier:
    def __init__(
            self,
            *,
            learning_rate=0.1,
            n_estimators=100,
            criterion="friedman_mse",
            min_samples_split=2,
            min_samples_leaf=1,
            max_depth=3,
            random_state=None
    ):
        self.learning_rate = learning_rate
        self.n_estimators = n_estimators
        self.criterion = criterion
        self.min_samples_split = min_samples_split
        self.min_samples_leaf = min_samples_leaf
        self.max_depth = max_depth
        self.random_state = random_state
        self.f0 = None
        self.coef = []
        self.trees = []

    def log_loss(self, x, y):
        proba = self.sigmoid(self.predict_proba(x))[:, 1]
        loss1 = np.sum(y * np.log(proba))
        loss2 = np.sum((1 - y) * np.log(1 - proba))
        return -(loss1 + loss2)

    def log_loss_grad(self, x, y):
        proba = self.predict_proba(x)[:, 1]
        return y - proba

    @staticmethod
    def sigmoid(z):
        return 1 / (1 + np.exp(-z))

    def fit(self, x, y):
        self.f0 = np.log(y.mean() / (1 - y.mean()))
        for t in range(self.n_estimators):
            pseudo_remainder = self.log_loss_grad(x, y)
            tree = DecisionTreeRegressor(criterion=self.criterion, min_samples_split=self.min_samples_split,
                                         min_samples_leaf=self.min_samples_leaf, max_depth=self.max_depth,
                                         random_state=self.random_state).fit(x, pseudo_remainder)
            tree_pred = tree.predict(x)
            proba = self.predict_proba(x)[:, 1]
            rho = (pseudo_remainder.T @ tree_pred) / np.sum(tree_pred**2 * proba * (1 - proba))
            self.coef.append(self.learning_rate * rho)
            self.trees.append(tree)

    def predict_proba(self, x):
        y_pred = self.f0
        if len(self.trees) != 0:
            trees_pred = np.vstack([tree.predict(x) for tree in self.trees]).T
            y_pred += trees_pred @ np.array(self.coef)
        return np.vstack([1 - self.sigmoid(y_pred), self.sigmoid(y_pred)]).T

    def predict(self, x):
        proba = self.predict_proba(x)
        return proba.argmax(axis=-1, keepdims=True)

    @property
    def estimators_(self):
        return self.trees

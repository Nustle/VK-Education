import numpy as np


def product_of_diagonal_elements_vectorized(matrix: np.array):
    diagonal_elements = matrix.diagonal()
    return np.prod(diagonal_elements[np.nonzero(diagonal_elements)[0]])


def are_equal_multisets_vectorized(x: np.array, y: np.array):
    if x.shape[0] != y.shape[0]:
        return False
    x.sort()
    y.sort()
    return np.all(x == y)


def max_before_zero_vectorized(x: np.array):
    zero_ind = np.where(x == 0)[0]
    if zero_ind[-1] == x.shape[0] - 1:
        zero_ind = zero_ind[:-1]
    return np.max(x[zero_ind + 1])


def add_weighted_channels_vectorized(image: np.array):
    height, width, _ = image.shape
    gray_coefficients = np.array([0.299, 0.587, 0.114]).reshape(3, 1)
    return np.dot(image, gray_coefficients).reshape(height, width)


def run_length_encoding_vectorized(x: np.array):
    diff_indices = np.diff(x)
    diff_mask = diff_indices != 0
    indices_of_end_series = np.concatenate((np.where(diff_mask)[0], np.array([x.size - 1])))
    nums = x[indices_of_end_series]
    indices_of_end_series = np.concatenate((np.array([-1]), indices_of_end_series))
    repeats = np.diff(indices_of_end_series)
    return nums, repeats

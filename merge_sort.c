#include "minishell.h"

void	merge_sorted_arrays(char **arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;
	int	k;
    char **L = malloc(n1 * sizeof(char *));
    char **R = malloc(n2 * sizeof(char *));

    int i = 0;
    while (i < n1) {
        L[i] = arr[left + i];
        i++;
    }

    int j = 0;
    while (j < n2) {
        R[j] = arr[mid + 1 + j];
        j++;
    }
    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
        if (ft_strcmp(L[i], R[j]) <= 0) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    free(L);
    free(R);
}

void	merge_sort(char **arr, int left, int right)
{
	int	mid;

    if (left < right) {
        mid = left + (right - left) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge_sorted_arrays(arr, left, mid, right);
    }
}

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void generate_combinations(vector<vector<int>> &board, vector<vector<long long>> &dp, long long act_value, int act_mask, int next_mask, int act_col, int act_row, int n, int k);

void process_column(vector<vector<int>> &board, vector<vector<long long>> &dp, int column, int n, int k) {
    for (int act_mask = 0; act_mask < 1 << k; ++act_mask) {
        if (dp[column][act_mask] >= 0) {
            generate_combinations(board, dp, dp[column][act_mask], act_mask, 0, column, 0, n, k);
        }
    }
}

void vertical_combination(vector<vector<int>> &board, vector<vector<long long>> &dp, long long act_value, int act_mask, int next_mask, int act_col, int act_row, int n, int k) {
    if (act_row < k - 1 && (act_mask & (1 << (act_row + 1))) == 0) {  // If the next row is not used
        int sum = board[act_row][act_col] + board[act_row + 1][act_col];
            if (sum > 0) {
                generate_combinations(board, dp, act_value + sum, act_mask, next_mask, act_col, act_row + 2, n, k);
            }
        }
}

void horizontal_combination(vector<vector<int>> &board, vector<vector<long long>> &dp, long long act_value, int act_mask, int next_mask, int act_col, int act_row, int n, int k) {
    if (act_col < n - 1) {
        int sum = board[act_row][act_col] + board[act_row][act_col + 1];
        if (sum > 0) {
            int next_mask2 = next_mask | (1 << act_row);
            generate_combinations(board, dp, act_value + sum, act_mask, next_mask2, act_col, act_row + 1, n, k);
        }
    }
}

void generate_combinations(vector<vector<int>> &board, vector<vector<long long>> &dp, long long act_value, int act_mask, int next_mask, int act_col, int act_row, int n, int k) {
    dp[act_col + 1][next_mask] = max(dp[act_col + 1][next_mask], act_value);
    for (int row = act_row; row < k; ++row) {
        if ((act_mask & (1 << row)) == 0) { // If the row is not used
            vertical_combination(board, dp, act_value, act_mask, next_mask, act_col, row, n, k);
            horizontal_combination(board, dp, act_value, act_mask, next_mask, act_col, row, n, k);
        }
    }
}



int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<int>> board(k, vector<int>(n));
    vector<vector<long long>> dp(n + 1, vector<long long>(1 << k, -1));

    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> board[i][j];
        }
    }

    dp[0][0] = 0;

    for (int i = 0; i < n; ++i) {
        process_column(board, dp, i, n, k);
    }

    cout << dp[n][0] << endl;

    return 0;
}
/* PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 * Shape of each piece is represented by rows in the array.
 * TIP: Name the array what is already been coded to avoid any unwanted errors.
 */
// Each row represents ONE Tetris piece
// Each number represents a block position in a 4x4 grid
/*
Muhammad Shoaib 2502866
Haider Ali Naushad 2502852
BSSE-F-25-B
*/
int BLOCKS[7][4] =
{
    {1, 3, 5, 7}, // I shape
    {2, 4, 5, 7}, // Z shape
    {3, 5, 4, 6}, // S shape
    {3, 5, 4, 7}, // T shape
    {2, 3, 5, 7}, // L shape
    {3, 5, 7, 6}, // J shape
    {2, 3, 4, 5}  // O shape (square)
};
/*[
*   *   *   *
*   *   *   *
*   *   *   *
*   *   *   *
]*/
/*
| BLOCKS[n][i] | x = %2 | y = /2 |
| ------------ | ------ | ------ |
| 0            | 1      | 0      |
| 0            | 1      | 1      |
| 0            | 1      | 2      |
| 0            | 1      | 3      |

*/
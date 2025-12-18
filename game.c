#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

#define BOARD_SIZE 8
#define MAX_DIRS 4

char board[BOARD_SIZE][BOARD_SIZE];
int L[BOARD_SIZE];
int S[4], F[4];
int V[16];
int N[4];

void initialize_game() {
    int v_values[] = {
        1, 100, 500, 1000000,      // Line 130
        1, 800, 4000, 1000000,     // Line 130 cont
        1, 75, 900, 1000000000,    // Line 140
        1, 450, 3000, 1000000000   // Line 140 cont
    };
    
    for (int i = 0; i < 16; i++) {
        V[i] = v_values[i];
    }
    
    // Initialize board
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = '-';
        }
        L[i] = 0;
    }
}

void print_board() {
    printf("\n");
    for (int i = BOARD_SIZE - 1; i >= 0; i--) {
        printf("  ");
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c   ", board[i][j]);
        }
        printf("\n");
    }
    
    printf("  ");
    for (int i = 1; i <= BOARD_SIZE; i++) {
        printf("%d   ", i);
    }
    printf("\n\n");
}

// Subroutine from line 1240-1570 in BASIC
void check_direction(int col, int row, char player, int dir_idx, int dx, int dy) {
    char opponent = (player == 'X') ? 'O' : 'X';
    int d = 1;
    int s = 1;
    int t = 0;
    int c = 0;
    
    // First pass
    for (int k = 1; k <= 3; k++) {
        int m5 = col + k * dx;
        int l1 = row + k * dy;
        
        if (m5 < 0 || l1 < 0 || m5 >= BOARD_SIZE || l1 >= BOARD_SIZE) {
            break;
        }
        
        char b = board[l1][m5];
        
        if (c == 0) {
            if (b == player) {
                s++;
            } else if (b == '-') {
                c = 1;
                t++;
            } else if (b == opponent) {
                break;
            }
        } else {
            if (b == opponent) {
                break;
            }
            t++;
        }
    }
    
    d = 0;
    dx = -dx;
    dy = -dy;
    c = 0;
    
    // Second pass (reverse direction)
    for (int k = 1; k <= 3; k++) {
        int m5 = col + k * dx;
        int l1 = row + k * dy;
        
        if (m5 < 0 || l1 < 0 || m5 >= BOARD_SIZE || l1 >= BOARD_SIZE) {
            break;
        }
        
        char b = board[l1][m5];
        
        if (c == 0) {
            if (b == player) {
                s++;
            } else if (b == '-') {
                c = 1;
                t++;
            } else if (b == opponent) {
                break;
            }
        } else {
            if (b == opponent) {
                break;
            }
            t++;
        }
    }
    
    S[dir_idx] = s;
    F[dir_idx] = t;
}

// GOSUB 1240 equivalent
void evaluate_position(int col, int row, char player) {
    check_direction(col, row, player, 0, 1, 0);   // Right
    check_direction(col, row, player, 1, 1, 1);   // Up-right
    check_direction(col, row, player, 2, 0, 1);   // Up
    check_direction(col, row, player, 3, -1, 1);  // Up-left
}

int check_win(char player) {
    for (int i = 0; i < 4; i++) {
        if (S[i] >= 4) {
            return 1;
        }
    }
    return 0;
}

int human_move() {
    int col;
    char input[10];
    
    while (1) {
        printf("A NUMBER BETWEEN 1 AND 8: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return -1;
        }
        
        col = atoi(input);
        if (col < 1 || col > BOARD_SIZE) {
            printf("ILLEGAL MOVE, TRY AGAIN.\n");
            continue;
        }
        
        col--;
        if (L[col] >= BOARD_SIZE) {
            printf("ILLEGAL MOVE, TRY AGAIN.\n");
            continue;
        }
        
        break;
    }
    
    int row = L[col];
    board[row][col] = 'X';
    L[col]++;
    
    return col;
}

int computer_move() {
    int best_col = -1;
    int best_value = -1;
    int candidates = 0;
    
    for (int col = 0; col < BOARD_SIZE; col++) {
        int row = L[col];
        if (row >= BOARD_SIZE) continue;
        
        int value = 1;
        
        // Evaluate for computer (O)
        char player = 'O';
        evaluate_position(col, row, player);
        
        // Reset N array
        for (int i = 0; i < 4; i++) N[i] = 0;
        
        int w = 0;
        
        // Check for immediate win
        for (int z = 0; z < 4; z++) {
            if (S[z] - w > 3) {
                best_col = col;
                printf("COMPUTER PICKS COLUMN %d\n\n", col + 1);
                board[row][col] = 'O';
                L[col]++;
                return col;
            }
        }
        
        // Evaluate threats and opportunities
        for (int z = 0; z < 4; z++) {
            int s = S[z];
            int t = s + F[z];
            
            if (t >= 4) {
                value += 4;
                if (s >= 1 && s <= 4) {
                    N[s - 1]++;
                }
            }
        }
        
        // Calculate heuristic value for computer
        for (int i = 0; i < 4; i++) {
            int n = N[i] - 1;
            if (n >= -1) {
                int sgn_n = (n > 0) ? 1 : ((n < 0) ? -1 : 0);
                int i1 = 8 * w + 4 * sgn_n + (i + 1);
                
                if (i1 >= 1 && i1 <= 16) {
                    value += V[i1 - 1];
                    if (n > 0) {
                        value += n * V[8 * w + i];
                    }
                }
            }
        }
        
        // Evaluate for opponent (X) at same position
        w = 1;
        player = 'X';
        evaluate_position(col, row, player);
        
        // Reset N for opponent evaluation
        for (int i = 0; i < 4; i++) N[i] = 0;
        
        for (int z = 0; z < 4; z++) {
            int s = S[z];
            int t = s + F[z];
            
            if (t >= 4) {
                value += 4;
                if (s >= 1 && s <= 4) {
                    N[s - 1]++;
                }
            }
        }
        
        // Calculate heuristic for opponent
        for (int i = 0; i < 4; i++) {
            int n = N[i] - 1;
            if (n >= -1) {
                int sgn_n = (n > 0) ? 1 : ((n < 0) ? -1 : 0);
                int i1 = 8 * w + 4 * sgn_n + (i + 1);
                
                if (i1 >= 1 && i1 <= 16) {
                    value += V[i1 - 1];
                    if (n > 0) {
                        value += n * V[8 * w + i];
                    }
                }
            }
        }
        
        // Check if opponent would win next move
        int next_row = row + 1;
        if (next_row < BOARD_SIZE) {
            evaluate_position(col, next_row, 'X');
            for (int z = 0; z < 4; z++) {
                if (S[z] > 3) {
                    value = 2;
                    break;
                }
            }
        }
        
        // Select best move
        if (value > best_value) {
            best_value = value;
            best_col = col;
            candidates = 1;
        } else if (value == best_value) {
            candidates++;
            if (rand() % candidates == 0) {
                best_col = col;
            }
        }
    }
    
    if (best_col == -1) {
        return -1;
    }
    
    printf("COMPUTER PICKS COLUMN %d\n\n", best_col + 1);
    board[L[best_col]][best_col] = 'O';
    L[best_col]++;
    
    return best_col;
}

void print_header() {
    printf("\n\t\t      FOUR IN A ROW\n");
    printf("\t\t    CREATIVE COMPUTING\n");
    printf("\t\t  MORRISTOWN, NEW JERSEY\n\n\n");
}

void print_instructions() {
    printf("THE GAME OF FOUR IN A ROW\n");
    printf("THE GAME CONSISTS OF STACKING X'S\n");
    printf("AND O'S (THE COMPUTER HAS O) UNTIL\n");
    printf("ONE OF THE PLAYERS GETS FOUR IN A\n");
    printf("ROW VERTICALLY, HORIZONTALLY, OR\n");
    printf("DIAGONALLY.\n\n");
}

int main() {
    srand(time(NULL));
    char input[10];
    
    print_header();
    
    printf("DO YOU WANT INSTRUCTIONS? (YES/NO): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    
    for (int i = 0; input[i]; i++) {
        input[i] = toupper(input[i]);
    }
    
    if (strcmp(input, "YES") == 0) {
        print_instructions();
    } else if (strcmp(input, "NO") != 0) {
        printf("Please answer YES or NO.\n");
        return 1;
    }
    
    initialize_game();
    
    printf("DO YOU WANT TO GO FIRST? (YES/NO): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    
    for (int i = 0; input[i]; i++) {
        input[i] = toupper(input[i]);
    }
    
    int player_turn;
    if (strcmp(input, "YES") == 0) {
        player_turn = 1;
    } else if (strcmp(input, "NO") == 0) {
        player_turn = 0;
    } else {
        printf("Please answer YES or NO.\n");
        return 1;
    }
    
    while (1) {
        if (player_turn) {
            print_board();
            int col = human_move();
            if (col == -1) continue;
            
            int row = L[col] - 1;
            evaluate_position(col, row, 'X');
            
            if (check_win('X')) {
                print_board();
                printf("Y O U   W I N !!!\n");
                break;
            }
            player_turn = 0;
        } else {
            int col = computer_move();
            if (col == -1) {
                print_board();
                printf("T I E   G A M E ...\n");
                break;
            }
            
            int row = L[col] - 1;
            evaluate_position(col, row, 'O');
            
            if (check_win('O')) {
                print_board();
                printf("C O M P U T E R   W I N S !!!\n");
                break;
            }
            player_turn = 1;
        }
    }
    
    return 0;
}
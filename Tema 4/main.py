from itertools import combinations

def print_board(board):
    for i in range(1, 10):
        if i in board:
            print(board[i], end=" | ")
        else:
            print(" ", end=" | ")
        if i % 3 == 0:
            print()

def is_final(board):
    if len(board) < 3:
        return False

    for player in ['A', 'B']:
        player_choices = [num for num in board if board[num] == player]
        if len(player_choices) < 3:
            continue

        for combination in combinations(player_choices, 3):
            if sum(combination) == 15:
                return True

    return len(board) == 9

def transition(board, move, player):
    if move < 1 or move > 9 or move in board:
        return False

    board[move] = player
    return True

def check_winner(board, player):
    if len(board) < 3:
        return False

    player_choices = [num for num in board if board[num] == player]
    if len(player_choices) < 3:
        return False

    for combination in combinations(player_choices, 3):
        if sum(combination) == 15:
            return True

    return False

def best_move_for_B(board, available_combinations):
    magic_square = [
        [2, 7, 6],
        [9, 5, 1],
        [4, 3, 8]
    ]

    max_intersection_count = 0
    best_move = None

    for num in range(1, 10):
        if num in board:
            continue

        intersection_count = 0
        for combination in available_combinations:
            if num in combination:
                intersection_count += 1

        if intersection_count > max_intersection_count:
            max_intersection_count = intersection_count
            best_move = num

    return best_move

def main():
    board = {}
    players = ['A', 'B']
    current_player = 0
    available_combinations = [(1, 5, 9),
                            (1, 9, 5),
                            (5, 1, 9),
                            (5, 9, 1),
                            (9, 1, 5),
                            (9, 5, 1),
                            (1, 6, 8),
                            (1, 8, 6),
                            (6, 1, 8),
                            (6, 8, 1),
                            (8, 1, 6),
                            (8, 6, 1),
                            (2, 4, 9),
                            (2, 9, 4),
                            (4, 2, 9),
                            (4, 9, 2),
                            (9, 2, 4),
                            (9, 4, 2),
                            (2, 5, 8),
                            (2, 8, 5),
                            (5, 2, 8),
                            (5, 8, 2),
                            (8, 2, 5),
                            (8, 5, 2),
                            (2, 6, 7),
                            (2, 7, 6),
                            (6, 2, 7),
                            (6, 7, 2),
                            (7, 2, 6),
                            (7, 6, 2),
                            (3, 4, 8),
                            (3, 8, 4),
                            (4, 3, 8),
                            (4, 8, 3),
                            (8, 3, 4),
                            (8, 4, 3),
                            (3, 5, 7),
                            (3, 7, 5),
                            (5, 3, 7),
                            (5, 7, 3),
                            (7, 3, 5),
                            (7, 5, 3),
                            (4, 5, 6),
                            (4, 6, 5),
                            (5, 4, 6),
                            (5, 6, 4),
                            (6, 4, 5),
                            (6, 5, 4)]

    while not is_final(board):
        print_board(board)
        if players[current_player] == 'A':
            num = int(input(f"Player A: Choose a number between 1 and 9: "))
        else:
            num = best_move_for_B(board, available_combinations)
            print(f"Player B: Choose a number between 1 and 9: {num}")
        if not transition(board, num, players[current_player]):
            print("Invalid choice. Try again.")
            continue

        if is_final(board):
            print_board(board)
            if check_winner(board, players[current_player]):
                print(f"Player {players[current_player]} wins!")
            else:
                print("It's a draw")

        if players[current_player] == 'B':
            # Actualizăm combinațiile disponibile
            available_combinations = [comb for comb in available_combinations if num not in comb]

        current_player = 1 - current_player

if __name__ == "__main__":
    main()

# Exemplu: A:3 B:9 A:5 B:7 A:2 B:8 A:4 B:1 A:6 A câștigă (6+5+4)
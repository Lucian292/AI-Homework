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

def best_move_for_B(board, depth):
    best_move = None
    best_score = float('-inf')
    for move in get_available_moves(board):
        new_board = board.copy()
        new_board[move] = 'B'
        score = MinMax(new_board, depth, 'B')
        if score > best_score:
            best_score = score
            best_move = move
            #print("best score", best_score, "best move", best_move)
    return best_move

def get_available_moves(board):
    # Inițializăm o listă goală pentru a stoca mișcările disponibile
    available_moves = []

    # Verificăm numerele de la 1 la 9
    for num in range(1, 10):
        if num not in board:
            available_moves.append(num)

    return available_moves

def MinMax(board, depth, player):
    if depth == 0 or is_final(board):
        eval_board = evaluate_board(board, player)
        #print("eval board", eval_board)
        return eval_board

    if player == 'B':
        best_score = float('-inf')
        for move in get_available_moves(board):
            new_board = board.copy()
            new_board[move] = player
            score = MinMax(new_board, depth - 1, 'A')
            best_score = max(best_score, score)
        return best_score
    else:
        best_score = float('inf')
        for move in get_available_moves(board):
            new_board = board.copy()
            new_board[move] = player
            score = MinMax(new_board, depth - 1, 'B')
            best_score = min(best_score, score)
        return best_score

def evaluate_board(board, player):
    if is_final(board):
        if check_winner(board, player):
            return 200  # Jucatorul B a castigat
        else:
            return 0  # Remiza

    b_score = 0

    blocking_combinations = [(1, 5, 9), (3, 5, 7), (2, 5, 8), (4, 5, 6), (2, 9, 4), (6, 1, 8), (2, 7, 6), (8, 3, 4)]

    for combination in blocking_combinations:
        count_b = 0
        count_a = 0
        #count_b = sum(1 for num in combination if board.get(num) == 'B')
        for num in combination:
            if num not in board:
                continue
            if board[num] == 'A':
                count_a += 1
                break
        for num in combination:
            if num not in board:
                continue
            if board[num] == 'B':
                count_b += 1
                break
        #count_a = sum(1 for num in combination if board.get(num) == 'A')

        #print(f"Combinația {combination}: A are {count_a} ocupate, B are {count_b} ocupate.")

        if (count_b == 1 or count_b == 2) and (count_a == 1 or count_a == 2):
            b_score -= 10
            #print(b_score, count_b, "primul if")
        if count_b == 1 and count_a == 0:
            b_score += 10
            #print(b_score, count_b, "al doilea if")
        if count_b == 3:
            #print(b_score, count_b, "al treilea if")
            return 200
            #b_score += 100

        if count_b == 2 and count_a == 0:
            #print(b_score, count_b, "al patrulea if")
            return 100
            #b_score += 10

    return b_score

def main():
    #global num
    board = {}
    players = ['A', 'B']
    current_player = 0


    depth = 2

    while not is_final(board):
        print_board(board)
        if players[current_player] == 'A':
            num = int(input(f"Player A: Choose a number between 1 and 9: "))
        else:
            if players[current_player] == 'B':
                num = best_move_for_B(board, depth)
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


        current_player = 1 - current_player

if __name__ == "__main__":
    main()


# Exemplu: A:3 B:9 A:5 B:7 A:2 B:8 A:4 B:1 A:6 A câștigă (6+5+4)
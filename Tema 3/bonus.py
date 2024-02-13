def solve_sudoku(sudoku_board):
    def is_valid_assignment(assignment, var, value):
        # Verificăm dacă valoarea atribuită variabilei var este validă în cadrul atribuirii curente
        row, col = var
        for i in range(9):
            if i != col and assignment[row][i] == value:
                return False
            if i != row and assignment[i][col] == value:
                return False

        # Verificăm regiunea 3x3
        region_row, region_col = row // 3, col // 3
        for i in range(region_row * 3, (region_row + 1) * 3):
            for j in range(region_col * 3, (region_col + 1) * 3):
                if i != row and j != col and assignment[i][j] == value:
                    return False

        return True

    def select_unassigned_variable(assignment):
        # Selectăm variabila neatribuită cu cel mai mic domeniu
        min_domain_size = 10  # Inițializăm cu o valoare mai mare decât oricare domeniu
        selected_var = None
        for row in range(9):
            for col in range(9):
                if assignment[row][col] <= 0:  # Modificare aici
                    domain_size = len(domains[(row, col)])
                    if domain_size < min_domain_size:
                        min_domain_size = domain_size
                        selected_var = (row, col)
        return selected_var

    def order_domain_values(var, assignment):
        row, col = var
        domain = domains[var]

        def count_constraints(value):
            return sum(1 for i in range(9) if i != col and (row, i) not in assignment and value in domains[(row, i)]) + \
                sum(1 for i in range(9) if i != row and (i, col) not in assignment and value in domains[(i, col)]) + \
                sum(1 for i in range(row // 3 * 3, row // 3 * 3 + 3) for j in range(col // 3 * 3, col // 3 * 3 + 3) if
                    (i, j) not in assignment and value in domains[(i, j)])

        # Prioritize pare
        return sorted(domain, key=lambda value: (value % 2, count_constraints(value)))

    def inference(assignment, var, value):
        inferences = set()
        for constraint in constraints[var]:
            for neighbor in constraint:
                if isinstance(neighbor, tuple) and assignment[neighbor[0]][neighbor[1]] == 0 and value in domains[neighbor]:
                    if len(domains[neighbor]) == 1:
                        return False
                    domains[neighbor].remove(value)
                    inferences.add(neighbor)
        return inferences

    def recursive_backtracking(assignment):
        if all(all(cell != 0 for cell in row) for row in assignment):
            return assignment  # Am completat toate celulele

        var = select_unassigned_variable(assignment)
        row, col = var

        for value in order_domain_values(var, assignment):
            if is_valid_assignment(assignment, var, value):
                assignment[row][col] = value
                inferences = inference(assignment, var, value)
                if inferences is not False:
                    result = recursive_backtracking(assignment)
                    if result is not None:
                        return result
                assignment[row][col] = 0  # Revertim atribuirea
                for neighbor in inferences:
                    domains[neighbor].add(value)
        return None  # Eșuăm

    # Inițializăm domeniile și constrângerile
    domains = {}
    constraints = {}

    for row in range(9):
        for col in range(9):
            if sudoku_board[row][col] == -1:
                domains[(row, col)] = {2, 4, 6, 8}
            elif sudoku_board[row][col] == 0:
                domains[(row, col)] = set(range(1, 10))
            else:
                domains[(row, col)] = {sudoku_board[row][col]}

    for row in range(9):
        for col in range(9):
            constraint = []
            for i in range(9):
                if i != col:
                    constraint.append((row, i))
                if i != row:
                    constraint.append((i, col))
            region_row, region_col = row // 3, col // 3
            for i in range(region_row * 3, (region_row + 1) * 3):
                for j in range(region_col * 3, (region_col + 1) * 3):
                    if i != row and j != col:
                        constraint.append((i, j))
            constraints[(row, col)] = constraint

    return recursive_backtracking(sudoku_board)

sudoku_board = [
    [8, 4, 0, 0, 5, 0, -1, 0, 0],
    [3, 0, 0, 6, 0, 8, 0, 4, 0],
    [0, 0, -1, 4, 0, 9, 0, 0, -1],
    [0, 2, 3, 0, -1, 0, 9, 8, 0],
    [1, 0, 0, -1, 0, -1, 0, 0, 4],
    [0, 9, 8, 0, -1, 0, 1, 6, 0],
    [-1, 0, 0, 5, 0, 3, -1, 0, 0],
    [0, 3, 0, 1, 0, 6, 0, 0, 7],
    [0, 0, -1, 0, 2, 0, 0, 1, 3]
]

    # [0, 0, 0, 0, 0, 0, 0, 3, 6],
    # [0, 0, 7, 5, 0, 0, 0, 0, 0],
    # [0, 0, 0, 2, 0, 0, 0, 0, 0],
    # [4, 0, 0, 0, 0, 0, 0, 0, 7],
    # [0, 0, 0, 4, 0, 8, 0, 0, 0],
    # [0, 6, 0, 0, 0, 0, 0, 0, 9],
    # [2, 0, 0, 0, 0, 0, 0, 0, 0],
    # [0, 0, 0, 0, 9, 7, 2, 0, 0],
    # [0, 8, 0, 0, 0, 0, 0, 0, 0]

solved_sudoku = solve_sudoku(sudoku_board)

if solved_sudoku:
    for row in solved_sudoku:
        print(row)
else:
    print("Nu există soluție pentru această problemă Sudoku.")

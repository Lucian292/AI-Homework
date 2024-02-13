def solve_sudoku(sudoku_board):
    def is_valid_assignment(assignment, var, value):
        # Verifică dacă o atribuire este validă pentru o celulă specifică a Sudoku
        row, col = var
        for i in range(9):
            if i != col and assignment[row][i] == value:
                return False
            if i != row and assignment[i][col] == value:
                return False

        region_row, region_col = row // 3, col // 3
        for i in range(region_row * 3, (region_row + 1) * 3):
            for j in range(region_col * 3, (region_col + 1) * 3):
                if i != row and j != col and assignment[i][j] == value:
                    return False

        return True

    def select_unassigned_variable(assignment):
        # Selectează variabila neatribuită cu cel mai mic domeniu
        min_domain_size = 10  # Inițializăm cu o valoare mai mare decât oricare domeniu
        selected_var = None
        for row in range(9):
            for col in range(9):
                if assignment[row][col] <= 0:
                    domain_size = len(domains[(row, col)])
                    if domain_size < min_domain_size:
                        min_domain_size = domain_size
                        selected_var = (row, col)
        return selected_var

    def order_domain_values(var, assignment): #indeplineste functia MRV (Minimum Remaining Values)
        # Ordonează domeniul valorilor posibile pentru o variabilă
        row, col = var
        domain = domains[var]

        def count_constraints(value):
            count = 0  # Inițializăm un contor pentru a număra restricțiile care se aplică valorii

            # Pentru fiecare coloană (i != col)
            for i in range(9):
                if i != col and assignment[row][i] == 0 and value in domains[(row, i)]:
                    # Dacă valoarea poate fi atribuită în această coloană
                    # (coloana nu este cea în care se face atribuirea curentă, celula nu este deja atribuită și valoarea este în domeniul acelei celule)
                    count += 1

            # Pentru fiecare rând (i != row)
            for i in range(9):
                if i != row and assignment[i][col] == 0 and value in domains[(i, col)]:
                    # Dacă valoarea poate fi atribuită în acest rând
                    # (rândul nu este cel în care se face atribuirea curentă, celula nu este deja atribuită și valoarea este în domeniul acelei celule)
                    count += 1

            region_row, region_col = row // 3, col // 3

            # Pentru fiecare celulă din regiunea 3x3 corespunzătoare
            for i in range(region_row * 3, (region_row + 1) * 3):
                for j in range(region_col * 3, (region_col + 1) * 3):
                    if i != row and j != col and assignment[i][j] == 0 and value in domains[(i, j)]:
                        # Dacă valoarea poate fi atribuită în acea celulă din regiune
                        # (celula nu este cea în care se face atribuirea curentă, celula nu este deja atribuită și valoarea este în domeniul acelei celule)
                        count += 1

            return count

        # Ordonează valorile în funcție de numărul de restricții și paritate
        return sorted(domain, key=lambda value: (count_constraints(value), value % 2))

    def inference(assignment, var, value): #indeplineste functia Forward Checking
        inferences = set()  # Inițializăm un set pentru a ține evidența celulelor unde s-au făcut inferențe

        # Pentru fiecare constrângere (constraint) asociată cu variabila `var`
        for constraint in constraints[var]:
            # Pentru fiecare vecin (neighbor) din constrângere
            for neighbor in constraint:
                if isinstance(neighbor, tuple):
                    row, col = neighbor
                    # Verificăm dacă vecinul (o altă celulă din tabel) este încă necompletat (atribuit cu 0) și dacă valoarea `value` este în domeniul variabilei respective

                    if assignment[row][col] == 0 and value in domains[neighbor]:
                        # Dacă cele două condiții sunt îndeplinite, atunci facem o inferență
                        # Inferența constă în eliminarea valorii `value` din domeniul variabilei respective și adăugarea vecinului în setul `inferences` pentru a ține evidența inferențelor

                        # De asemenea, verificăm dacă domeniul vecinului a devenit vid (are doar această valoare)
                        if len(domains[neighbor]) == 1:
                            return False  # Dacă domeniul a devenit vid, nu putem continua și întoarcem `False`

                        domains[neighbor].remove(value)  # Eliminăm valoarea din domeniul vecinului
                        inferences.add(neighbor)  # Adăugăm vecinul în setul de inferențe

        return inferences

    def recursive_backtracking(assignment):
        # Verificăm dacă toate celulele sunt completate (nu mai avem 0-uri în tabel)
        if all(all(cell != 0 for cell in row) for row in assignment):
            return assignment  # Dacă da, am găsit o soluție și o returnăm

        var = select_unassigned_variable(assignment)  # Selectăm o variabilă neatribuită
        row, col = var

        for value in order_domain_values(var, assignment):
            if is_valid_assignment(assignment, var, value):
                assignment[row][col] = value  # Atribuim valoarea validă la variabila curentă

                inferences = inference(assignment, var, value)  # Aplicăm inferențele

                if inferences is not False:
                    result = recursive_backtracking(assignment)  # Continuăm explorarea recursivă
                    if result is not None:
                        return result  # Dacă găsim o soluție, o returnăm

                assignment[row][col] = 0  # Dacă nu am găsit o soluție, revenim la valoarea 0
                for neighbor in inferences:
                    domains[neighbor].add(value)  # Restaurăm domeniile celulelor inferate

        return None  # Dacă nu am găsit nicio soluție, returnăm None

    # Inițializarea domeniilor și constrângerilor
    domains = {}
    constraints = {}

    for row in range(9):
        for col in range (9):
            if sudoku_board[row][col] == -1:
                domains[(row, col)] = {2, 4, 6, 8}  # Celulele care trebuie să conțină numere pare
            elif sudoku_board[row][col] == 0:
                domains[(row, col)] = set(range(1, 10))  # Celulele care trebuie completate
            else:
                domains[(row, col)] = {sudoku_board[row][col]}

    for row in range(9):
        for col in range(9):
            constraint = []  # Inițializăm o listă pentru a reține constrângerile pentru celula curentă
            # Adăugăm constrângerile referitoare la coloane (toate celulele de pe același rând, excludând celula curentă)
            for i in range(9):
                if i != col:
                    constraint.append((row, i))
            # Adăugăm constrângerile referitoare la rânduri (toate celulele de pe aceeași coloană, excludând celula curentă)
            for i in range(9):
                if i != row:
                    constraint.append((i, col))
            region_row, region_col = row // 3, col // 3
            # Adăugăm constrângerile referitoare la regiunea 3x3 din care face parte celula curentă
            for i in range(region_row * 3, (region_row + 1) * 3):
                for j in range(region_col * 3, (region_col + 1) * 3):
                    if i != row and j != col:
                        constraint.append((i, j))
            constraints[(
            row, col)] = constraint  # Salvăm lista de constrângeri în dicționarul de constrângeri pentru celula curentă

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

solved_sudoku = solve_sudoku(sudoku_board)

if solved_sudoku:
    for row in solved_sudoku:
        print(row)
else:
    print("Nu există soluție pentru această problemă Sudoku.")

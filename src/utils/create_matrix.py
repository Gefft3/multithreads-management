import random
import sys


def create_matrix(rows, cols):
    """
    Creates a matrix with the specified number of rows and columns, filled with random integers.
    """
    
    return [[random.randint(1, 100) for _ in range(cols)] for _ in range(rows)]


def write_matrix_to_file(matrix, filename):
    """
    Writes the matrix to a file with the specified filename.
    """

    with open(filename, "w") as file:
        for row in matrix:
            file.write(" ".join(map(str, row)) + "\n")


def main(rows_a, cols_a, matrix_a_filename, rows_b, cols_b, matrix_b_filename):
    """
    Main function to create two matrices and save them to files.
    It checks if the number of columns in Matrix A equals the number of rows in Matrix B.
    """

    if cols_a != rows_b:
        print(
            "Erro: O número de colunas da Matriz A deve ser igual ao número de linhas da Matriz B."
        )
        return

    matrix_a = create_matrix(rows_a, cols_a)
    matrix_b = create_matrix(rows_b, cols_b)

    write_matrix_to_file(matrix_a, matrix_a_filename)
    write_matrix_to_file(matrix_b, matrix_b_filename)
    print(f"Matriz A salva em {matrix_a_filename}")
    print(f"Matriz B salva em {matrix_b_filename}")


if __name__ == "__main__":
    try:
        (
            _,
            matrix_a_rows,
            matrix_a_cols,
            matrix_a_filename,
            matrix_b_rows,
            matrix_b_cols,
            matrix_b_filename,
        ) = sys.argv
        main(
            int(matrix_a_rows),
            int(matrix_a_cols),
            matrix_a_filename,
            int(matrix_b_rows),
            int(matrix_b_cols),
            matrix_b_filename,
        )
        print("Matriz A e Matriz B criadas e salvas com sucesso.")

    except ValueError as e:
        print(f"Erro: {e}")
        print(
            "Uso correto: python create_matrix.py <rows_a> <cols_a> <matrix_a_filename> <rows_b> <cols_b> <matrix_b_filename>"
        )

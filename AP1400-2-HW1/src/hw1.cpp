#include <cstddef>
#include <stdexcept>
#include <random>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <algorithm>

#include "hw1.h"

using T = double;
using Array = std::vector<T>;
using Matrix = std::vector<Array>;

struct Mat_size
{
    size_t n, m;
};

Mat_size getsize(const Matrix& mat)
{
    return {mat.size(), mat.front().size()};
}

void valid(const Matrix& mat)
{
    size_t m = mat[0].size();
    for(const Array& row : mat){
        if(row.size() != m){
            throw std::logic_error("Invalid size");
        }
    }
}

void valid(size_t n, size_t m)
{
    if(!n && !m){
        throw std::logic_error("Invalid size");
    }
}

int valid(const Matrix& mat1, const Matrix& mat2)
{
    int c1 = mat1.empty(), c2 = mat2.empty();
    if(c1 && (!c2) || (!c1) && c2){
        throw std::logic_error("Invalid multiply");
    }
    return !(c1 && c2);
}


namespace algebra
{
    Matrix zeros(size_t n, size_t m)
    {
        valid(n, m);
        return Matrix(n, Array(m, 0));
    }

    Matrix ones(size_t n, size_t m)
    {
        valid(n, m);
        return Matrix(n, Array(m,1));
    }

    Matrix random(size_t n, size_t m, double min, double max)
    {
        valid(n, m);

        if(min >= max){
            throw std::logic_error("Error : min >= max");
        }

        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<> distrib(min, max-1);

        Matrix mat(n);
        for(int i=0; i<n; i++){
            Array arr(m);
            for(int j=0; j<m; j++){
                arr[j] = distrib(gen);
            }
            mat[i] = arr;
        }
        return mat;
    }

    void show(const Matrix& matrix)
    {
        valid(matrix);

        std::cout << std::fixed;
        std::cout << std::setprecision(3);

        auto [row, col] = getsize(matrix);
        for(const auto& arr : matrix){
            for(const auto& elem : arr){
                std::cout << elem << " ";
            }
            std::cout << std::endl;
        }
    }

    Matrix multiply(const Matrix& matrix, double c)
    {
        valid(matrix);
        if(matrix.empty()){
            return matrix;
        }

        double epsilon = 1e-10;
        if(std::abs(c) < epsilon){
            throw std::logic_error("Invalid c");
        }

        Matrix mat = matrix;
        for(auto& arr : mat){
            for(auto& elem : arr){
                elem *= c;
            }
        }

        return mat;
    }

    Matrix multiply(const Matrix& matrix1, const Matrix& matrix2)
    {
        if(!valid(matrix1, matrix2)){
            return Matrix();
        }

        auto [r1, c1] = getsize(matrix1);
        auto [r2, c2] = getsize(matrix2);

        if(c1 != r2){
            throw std::logic_error("Invalid multiply");
        }

        Matrix mat(r1, Array(c2, 0));

        for(int i=0; i<r1; i++){
            for(int j=0; j<c2; j++){
                for(int k=0; k<c1; k++){
                    mat[i][j] += matrix1[i][k] * matrix2[k][j];
                }
            }
        }
        return mat;

    }

    Matrix sum(const Matrix& matrix, double c)
    {
        if(matrix.empty()){
            return matrix;
        }

        valid(matrix);

        auto [row, col] = getsize(matrix);

        Matrix mat = matrix;
        for(auto& arr : mat){
            for(auto& elem : arr){
                elem += c;
            }
        }
        return mat;
    }

    Matrix sum(const Matrix& matrix1, const Matrix& matrix2)
    {
        if(!valid(matrix1, matrix2)){
            return Matrix();
        }

        auto [row, col] = getsize(matrix1);

        Matrix mat(row, Array(col, 0));
        for(int i=0; i<row; i++){
            for(int j=0; j<col; j++){
                mat[i][j] = matrix1[i][j] + matrix2[i][j];
            }
        }
        return mat;
    }

    Matrix transpose(const Matrix& matrix)
    {

        if(matrix.empty()){
            return Matrix();
        }

        valid(matrix);

        auto [row, col] = getsize(matrix);

        Matrix mat = zeros(col, row);

        for(int i=0; i<row; i++){
            for(int j=0; j<col; j++){
                mat[j][i] = matrix[i][j];
            }
        }
        return mat;
    }

    Matrix minor(const Matrix& matrix, size_t n, size_t m)
    {
        valid(matrix);

        auto [row, col] = getsize(matrix);
        if(n < 0 || m < 0 || row < n || col < m){
            throw std::logic_error("minor error");
        }

        Matrix mat;
        for(int i=0; i<row; i++){
            if(i == n) continue;
            Array arr;
            for(int j=0; j<col; j++){
                if(j == m) continue;
                arr.push_back(matrix[i][j]);
            }
            mat.push_back(arr);
        }
        return mat;
    }

    double determinant(const Matrix& matrix)
    {
        if(matrix.empty()){
            return 1;
        }

        valid(matrix);

        auto [c, _] = getsize(matrix);

        if(c != _){
            throw std::logic_error("Error");
        }

        if(c == 1){
            return matrix[0][0];
        }else if(c == 2){
            return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
        }

        double res = 0;

        for(int i=0; i<c; i++){
            res += (i%2 == 0 ? 1.0 : -1.0) * matrix[0][i] * determinant(minor(matrix, 0, i));
        }

        return res;
    }

    Matrix inverse(const Matrix& matrix)
    {
        if(matrix.empty()){
            return Matrix();
        }

        auto [row, col] = getsize(matrix);

        if(row != col){
            throw std::logic_error("Invalid matrix");
        }

        if(!determinant(matrix)){
            throw std::logic_error("determinant is zero");
        }

        Matrix mat = zeros(row, col);
        for(int i=0; i<row; i++){
            for(int j=0; j<col; j++){
                mat[j][i] = ((i+j) % 2 == 0 ? 1.0 : -1.0) * determinant(minor(matrix, i, j));
            }
        }

        double det_inverse = 1.0 / determinant(matrix);
        for(auto& row : mat){
            for(auto& elem : row){
                elem *= det_inverse;
            }
        }

        return mat;
    }

    Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis){
        if(matrix1.empty() || matrix2.empty()){
            throw std::logic_error("Invalid concatenate");
        }

        valid(matrix1);
        valid(matrix2);

        auto [r1, c1] = getsize(matrix1);
        auto [r2, c2] = getsize(matrix2);

        Matrix mat = matrix1;

        if(axis == 0){
            if(c1 != c2){
                throw std::logic_error("c1 != c2");
            }
            for(auto& row : matrix2){
                mat.push_back(row);
            }
        }else if(axis == 1){
            if(r1 != r2){
                throw std::logic_error("r1 != r2");
            }
            for(int i=0; i<r1; i++){
                mat[i].reserve(c1 + c2);
                mat[i].insert(mat[i].end(), matrix2[i].begin(), matrix2[i].end());
            }
        }else{
            throw std::logic_error("Invalid axis");
        }
        return mat;
    }

    Matrix ero_swap(const Matrix& matrix, size_t r1, size_t r2)
    {
        if(matrix.empty()){
            return Matrix();
        }

        valid(matrix);

        auto [row, _] = getsize(matrix);

        if(r1 < 0 || r1 > row-1 || r2 < 0 || r2 > row-1){
            throw std::logic_error("Invalid row");
        }

        if(r1 == r2){
            return matrix;
        }

        Matrix mat = matrix;
        std::swap(mat[r1], mat[r2]);
        return mat;
        
    }

    Matrix ero_multiply(const Matrix& matrix, size_t r, double c)
    {
        if(matrix.empty()){
            return Matrix();
        }

        valid(matrix);

        auto [row, _] = getsize(matrix);

        if(r < 0 || r > row-1){
            throw std::logic_error("Invalid row");
        }

        Matrix mat = matrix;
        std::transform(mat[r].begin(), mat[r].end(), mat[r].begin(), [c](int elem){
            return elem * c;
        });
        return mat;
    }

    Matrix ero_sum(const Matrix& matrix, size_t r1, double c, size_t r2)
    {
        if(matrix.empty()){
            return Matrix();
        }

        valid(matrix);

        auto [row, _] = getsize(matrix);

        if(r1 < 0 || r1 > row-1 || r2 < 0 || r2 > row-1){
            throw std::logic_error("Invalid row");
        }

        Matrix mat = matrix;
        
        std::transform(mat[r1].begin(), mat[r1].end(), mat[r2].begin(), mat[r2].begin(), [c](int elem1, int elem2){
            return elem1 * c + elem2;
        });

        return mat;
    }

    Matrix upper_triangular(const Matrix& matrix)
    {
        if(matrix.empty()){
            return Matrix();
        }

        valid(matrix);

        auto [row, col] = getsize(matrix);
        if(row != col){
            throw std::logic_error("Invalid size when upper_triangular");
        }

        Matrix mat = matrix;

        for(int pivotRow = 0; pivotRow < row; pivotRow++){

            if(mat[pivotRow][pivotRow] == 0){
                int nonZeroRow = pivotRow + 1;
                while(nonZeroRow < row && mat[nonZeroRow][pivotRow] == 0){
                    nonZeroRow++;
                }

                if(nonZeroRow < row){
                    mat = ero_swap(mat, pivotRow, nonZeroRow);
                }else{
                    continue;
                }
            }

            for(int r = pivotRow + 1; r < row; r++){
                double multiple = -mat[r][pivotRow] / mat[pivotRow][pivotRow];
                mat = ero_sum(mat, pivotRow, multiple, r);
            }
        }
        return mat;

    }


}
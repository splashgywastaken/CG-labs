#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
//#include <debugapi.h>
#include <ostream>

template <class T>
class matrix
{
	std::vector<std::vector<T>> matrix_;
	
	//util functions
	static T& prod_of_sum (const std::vector<T>& a, const std::vector<T>& b)
	{
		auto result = new T(0);

		for (size_t i = 0; i < a.size(); i++)
			*result += a[i] * b[i];

		return *result;
	}

	static std::vector<T>& get_column(matrix& obj, const size_t& column_number)
	{
		const auto column = new std::vector<T>;
		column->resize(obj.n());

		for (size_t i = 0; i < obj.n(); i++)
			(*column)[i] = obj[i][column_number];

		return *column;
	}

public:
	explicit matrix(const std::vector<std::vector<T>> matrix)
	{
		matrix_ = std::move(matrix);
	}

	explicit matrix(const std::vector<T> matrix)
	{
		matrix_.resize(1);
		matrix_[0] = std::move(matrix);
	}

	matrix(const T** matrix, const size_t n)
	{

		matrix_.resize(n);
		for (size_t i = 0; i < n; i++)
			matrix_[i].resize(n);

		for (size_t i = 0; i < n; i++)
		{
			for (size_t j = 0; j < n; j++)
				matrix_[i][j] = matrix[i][j];
		}

	}

	matrix(const T** matrix, const size_t n, const size_t m)
	{

		matrix_.resize(n);
		for (size_t i = 0; i < n; i++)
			matrix_[i].resize(m);

		for (size_t i = 0; i < n; i++)
		{
			for (size_t j = 0; j < m; j++)
				matrix_[i][j] = matrix[i][j];
		}

	}

	matrix(const size_t& n, const size_t& m)
	{
		matrix_.resize(n);

		for (size_t i = 0; i < n; i++)
			matrix_[i].resize(m);

		for (auto& row : matrix_)
			for (auto& element : row)
				element = 0;

	}

	matrix(const matrix& other)
		: matrix_(other.matrix_)
	{
	}

	matrix(matrix&& other) noexcept
		: matrix_(std::move(other.matrix_))
	{
	}

	~matrix() = default;

	matrix& operator=(const matrix& other)
	{
		if (this == &other)
			return *this;
		matrix_ = other.matrix_;
		return *this;
	}

	matrix& operator=(matrix&& other) noexcept
	{
		if (this == &other)
			return *this;
		matrix_ = std::move(other.matrix_);
		return *this;
	}

	void set_ones()
	{
		try
		{

			if (n == m)
				throw std::exception("This is not a square matrix, try standart set method");

			for (size_t i = 0; i < n(); i++)
				matrix_[i][i] = 1;

		} catch (std::exception& e)
		{
			//OutputDebugStringA(e.what());
		}
	}

	void clear()
	{
		for (auto& row : matrix_)
			for (auto& element : row)
				element = 0;
	}

	void set(const std::vector<T>& array)
	{
		size_t i = 0;
		size_t j = 0;

		for (auto& element : array)
		{
			matrix_[i][j] = element;
			if (j < m())
				j++;
			else
			{
				i++;
				j = 0;
			}
		}
	}

	size_t m(){
		return
			matrix_[0].size();
	}

	size_t n()
	{
		return
			matrix_.size();
	}

	matrix& transpose()
	{
		auto transposed_matrix = new matrix<T>(m(), n());

		for (size_t i = 0; i < n(); i++)
			for (size_t j = 0; j < m(); j++)
				(*transposed_matrix)[j][i] = (*this)[i][j];

		return *transposed_matrix;
	}

	friend double det(matrix matr)
	{
		size_t n = matr.n();
		const size_t m = matr.m();

		if (m == 1)
		{
			return matr[0][0];
		}
		if (m == 2)
		{
			return matr[0][0] * matr[1][1] - matr[0][1] * matr[1][0];
		}

		double determinant = 0;
		int degree = 1;

		std::vector<std::vector<T>> minor(n);
		for (size_t i = 0; i < n - 1; i++)
			minor.resize(m - 1);

		for (size_t j = 0; j < n - 1; j++)
		{
			minor = get_matrix_without_row_column(matr, 0, j);
			determinant = determinant + matr[0][j] * static_cast<T>(degree) * det(matrix(minor));
			degree = -degree;
		}			

		return determinant;

	}

	matrix& inv()
	{

		return
			this->transpose() * (1 / det(*this));

	}

	friend bool operator==(matrix& lhs, matrix& rhs)
	{
		if (lhs.m() != rhs.m() || lhs.n() != rhs.n())
			return false;

		for (size_t i = 0; i < rhs.n(); i++)
			for (size_t j = 0; j < rhs.m(); j++)
				if (lhs[i][j] != rhs[i][j])
					return false;

		return true;
	}

	friend bool operator!=(matrix& lhs, matrix& rhs)
	{
		return !(lhs == rhs);
	}

	matrix& operator+(matrix& obj)
	{
		try
		{

			if (this->m() != obj.m() || this->n() != obj.n())
				throw std::exception("Matrixes spaces are not equal");

			const size_t n = this->n();
			const size_t m = this->m();

			for (size_t i = 0; i < n; i++)
				for (size_t j = 0; j < m; j++)
					this->matrix_[i][j] += obj.matrix_[i][j];

		} catch (std::exception& e)
		{
			//OutputDebugStringA(e.what());
		}

		return *this;
	}

	matrix& operator-(matrix& obj)
	{
		return obj * -1 + *this;
	}

	matrix& operator*(const int& k)
	{
		for (auto& row : matrix_)
			for (auto& element : row)
				element *= static_cast<T>(k);
		return *this;
	}

	matrix& operator*(const double& k)
	{
		for (auto& row : matrix_)
			for (auto& element : row)
				element *= static_cast<T>(k);
		return *this;
	}

	matrix& operator/(const int& k)
	{
		for (auto& row : matrix_)
			for (auto& element : row)
				element /= static_cast<T>(k);
		return *this;
	}

	matrix& operator/(const double& k)
	{
		for (auto& row : matrix_)
			for (auto& element : row)
				element /= static_cast<T>(k);
		return *this;
	}

	matrix& operator*(matrix& obj)
	{
		auto temp = obj.transpose();

		//obj - 3xN
		//this affine matrix - 3x3
		auto c = new matrix<T>(this->n(), temp.m());
		try
		{
			if (this->m() != temp.n())
				throw std::exception("Number of columns of first matrix is not equal to number of rows of second matrix\nOutputing null matrix");

			/*for (size_t i = 0; i < n(); i++)
				for (size_t j = 0; j < obj.n(); j++)
					(*c)[i][j] = prod_of_sum(get_column(obj, i), matrix_[j]);*/

			for (size_t i = 0; i < c->n(); i++)
				for (size_t j = 0; j < c->m(); j++)
					(*c)[i][j] = prod_of_sum((*this)/*.transpose()*/[i], get_column(temp, j));

		} catch(std::exception& e)
		{
				//OutputDebugStringA(e.what());
		} 

		return c->transpose();
	}

	std::vector<T>& operator[](const size_t& i)
	{
		return
			matrix_[i];
	}

	friend std::ostream& operator<<(std::ostream& os, matrix& obj)
	{
		const size_t n = obj.n();
		const size_t m = obj.m();

		for (size_t i = 0; i < n; i++)
		{
			for (size_t j = 0; j < m; j++)
			{
				os << obj[i][j] << "\t";
			}
			os << "\n";
		}

		return os;

	}

private:
	static std::vector<std::vector<T>> get_matrix_without_row_column(
		matrix obj, const size_t row, const size_t column 
	)
	{
		size_t n = obj.n() - 1;
		size_t m = obj.m() - 1;

		std::vector<std::vector<T>> new_matrix(n);
		for (size_t i = 0; i < n; i++)
			new_matrix[i].resize(m);

		size_t offset_row = 0;
		size_t offset_column = 0;

		for (size_t i = 0; i < n; i++)
		{
			if(i == row)
			{
				offset_row = 1;
			}		

			for (size_t j = 0; j < m; j++)
			{

				if (j == column)
					offset_column = 1;

				new_matrix[i][j] = obj[i + offset_row][j + offset_column];

			}
		}

		return new_matrix;
	}
};

#endif
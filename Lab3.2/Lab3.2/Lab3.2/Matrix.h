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
	static T prod_of_sum (const std::vector<T>& a, const std::vector<T>& b)
	{
		T result = 0;

		for (size_t i = 0; i < a.size(); i++)
			result += a[i] * b[i];

		return result;
	}

	static std::vector<T> get_column(matrix& obj, const size_t& column_number)
	{
		std::vector<T> column;
		column.resize(obj.n());

		for (size_t i = 0; i < obj.n(); i++)
			column[i] = obj[i][column_number];

		return column;
	}

public:

	matrix<T>() = default;

	~matrix()
	{
		const auto _n = new size_t(n());
		for (size_t i = 0; i < *_n; i++)
		{
			matrix_.at(i).clear();
		}
		matrix_.clear();
	}

	explicit matrix(const std::vector<std::vector<T>> matrix) : matrix_(std::move(matrix))
	{
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

	matrix(const size_t& n, const size_t& m) : matrix_(std::vector<std::vector<T>>(n, std::vector<T>(m)))
	{
	}

	matrix(const matrix& other)
		: matrix_(other.matrix_)
	{
	}

	matrix(matrix&& other) noexcept
		: matrix_(std::move(other.matrix_))
	{
	}
	
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

	matrix transpose()
	{
		auto transposed_matrix = new matrix<T>(m(), n());
		const auto matrix_n = new size_t(n());
		const auto matrix_m = new size_t(m());

		for (size_t i = 0; i < *matrix_n; i++)
			for (size_t j = 0; j < *matrix_m; j++)
				(*transposed_matrix)[j][i] = (*this)[i][j];

		delete matrix_n;
		delete matrix_m;

		return *transposed_matrix;
	}

	friend double det(matrix input_matrix)
	{
		size_t n = input_matrix.n();
		const size_t m = input_matrix.m();

		if (m == 1)
		{
			return input_matrix[0][0];
		}
		if (m == 2)
		{
			return input_matrix[0][0] * input_matrix[1][1] - input_matrix[0][1] * input_matrix[1][0];
		}

		double determinant = 0;
		int degree = 1;

		std::vector<std::vector<T>> minor(n);
		for (size_t i = 0; i < n - 1; i++)
			minor.resize(m - 1);

		for (size_t j = 0; j < n - 1; j++)
		{
			minor = get_matrix_without_row_column(input_matrix, 0, j);
			determinant = determinant + input_matrix[0][j] * static_cast<T>(degree) * det(matrix(minor));
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

	matrix<T> operator*(matrix<T>& obj)
	{

		if (m() == obj.n())
		{
			const auto new_n = new size_t(n());
			const auto new_m = new size_t(obj.m());
			const auto matrix_m = new size_t(m());
			matrix<T> result(*new_n, *new_m);

			for (size_t i = 0; i < *new_n; i++)
			{
				for (size_t j = 0; j < *new_m; j++)
				{

					for (size_t k = 0; k < *matrix_m; k++)
						result[i][j] += this->operator[](i)[k] * obj[k][j];

				}
			}

			delete new_n;
			delete new_m;
			delete matrix_m;

			return result;

		}

		return *this;

	}

	std::vector<T>& operator[](const int& i)
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
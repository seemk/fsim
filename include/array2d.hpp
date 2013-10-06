#include <vector>
#include <algorithm>

template<class T>
class Array2D
{
public:
	typedef Array2D<T>                               this_type;
	typedef std::vector<T>                           data_type;
	typedef typename std::vector<T>::value_type      value_type;
	typedef typename std::vector<T>::size_type       size_type;
	typedef typename std::vector<T>::reference       reference;
	typedef typename std::vector<T>::const_reference const_reference;

	Array2D() : m_rows(0U), m_cols(0U) {}

	// zero/value initialized matrix rows*cols
	Array2D(size_type rows, size_type cols)
		: m_rows(rows), m_cols(cols), m_data(rows*cols, T())
	{}

	// copy initialized matrix rows*cols
	Array2D(size_type rows, size_type cols, const_reference val)
		: m_rows(rows), m_cols(cols), m_data(rows*cols, val)
	{}

	// element access
	reference operator() (size_type row, size_type column)
	{
		return m_data[m_cols*row + column]; // row major indexation
	}

	const_reference operator() (size_type row, size_type column) const
	{
		return m_data[m_cols*row + column]; // row major indexation
	}

	reference operator[](size_type index)
	{
		return m_data[index];
	}

	const_reference operator[](size_type index) const
	{
		return m_data[index];
	}

	void resize(size_type new_rows, size_type new_cols)
	{
		using std::begin;
		this_type tmp(new_rows, new_cols);
		for (size_t i(0U); (i < new_rows) && (i < m_rows); ++i)
		{
			std::copy(begin(m_data) + i*m_cols,
				begin(m_data) + i*m_cols + std::min(m_cols, new_cols),
				begin(tmp.m_data) + i*tmp.cols());
		}
		this->swap(tmp);
	}

	size_type rows() const { return m_rows; }
	size_type cols() const { return m_cols; }

	void swap(this_type &rhs)
	{
		m_data.swap(rhs.m_data);
		std::swap(m_rows, rhs.m_rows);
		std::swap(m_cols, rhs.m_cols);
	}
private:

	size_type m_rows, m_cols;
	data_type m_data;
};

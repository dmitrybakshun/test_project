#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <complex>
#include "defines.h"
using namespace std;

template <typename Type>
void operator << (ostream& out, const vector<Type>& vec) {
	for (const auto& it : vec) {
		cout << it << endl;
	}
}

template <class Type>
vector<Type> operator *= (vector<Type>& vec1, vector<Type>& vec2)
{
	auto it2 = vec2.begin();
	for (auto& it1 : vec1) {
		it1 *= *it2;
		it2++;
	}
	return vec1;
}

enum class Win {
	BLACKMAN,
	HANN,
	HAMMING,
	HARRIS,
	NUTTALL,
	FLATTOP
};

template <class Type>
class Window
{
public:
	Window();
	
	void setType(Win type);
	void setSample(uint64_t sample);
	void proces(vector<Type>& vec);
	void reset();
	vector<Type> getWindow();


private:
	Win m_type;//тип
	uint64_t m_sample;//количество элементов
	vector<Type> m_vector;//вектор окна

	Type m_phase = 0.0;//фаза
	Type m_step = 0.0;//шаг 

	void init();
	void calcWindow();

	void blackman(vector<Type>& vec);
	void hann(vector<Type>& vec);
	void hamming(vector<Type>& vec);
	void harris(vector<Type>& vec);
	void nuttall(vector<Type>& vec);
	void flattop(vector<Type>& vec);
};

template <class Type>
Window<Type>::Window()
{

}

//¬вод, вывод, расчет---------------------------------
template <class Type>
inline void Window<Type>::setType(Win type)
{
	m_type = type;
	init();
}

template<class Type>
inline void Window<Type>::setSample(uint64_t sample)
{
	m_sample = sample;
	init();
}

template<class Type>
inline vector<Type> Window<Type>::getWindow()
{
	return m_vector;
}

template<class Type>
void Window<Type>::calcWindow()
{
	switch (m_type)
	{
	case Win::BLACKMAN:
		blackman(m_vector);
		break;
	case Win::HANN:
		hann(m_vector);
		break;
	case Win::HAMMING:
		hamming(m_vector);
		break;
	case Win::HARRIS:
		harris(m_vector);
		break;
	case Win::NUTTALL:
		nuttall(m_vector);
		break;
	case Win::FLATTOP:
		flattop(m_vector);
	default:
		break;
	}
}

template<class Type>
inline void Window<Type>::reset()
{
	m_phase = 0.0;
}

template<class Type>
inline void Window<Type>::init()
{
	m_vector.resize(m_sample);
	m_step = static_cast<Type>(2.0 * M_PI) /
		static_cast<Type>(m_sample);
	reset();
	calcWindow();
}

//ќкна-----------------------------------------------
template <class Type>
inline void Window<Type>::blackman(vector<Type>& vec)
{
	for (auto& it : vec) {
		it = 0.42 - 0.5 * cos(m_phase) +
			0.08 * cos(2.0 * m_phase);
		m_phase += m_step;
	}
	//vec[0] = 0.0;//!
}

template <class Type>
inline void Window<Type>::hann(vector<Type>& vec)
{
	for (auto& it : vec) {
		it = 0.5 * (1.0 - cos(m_phase));
		m_phase += m_step;
	}
}

template <class Type>
inline void Window<Type>::hamming(vector<Type>& vec)
{
	for (auto& it : vec) {
		it = 0.54 - 0.46 * cos(m_phase);
		m_phase += m_step;
	}
}

template <class Type>
inline void Window<Type>::harris(vector<Type>& vec)
{
	for (auto& it : vec) {
		it = 0.35875 - 0.48829 * cos(m_phase)
			+ 0.14128 * cos(2.0 * m_phase)
			- 0.01168 * cos(3.0 * m_phase);
		m_phase += m_step;
	}
}

template <class Type>
inline void Window<Type>::nuttall(vector<Type>& vec)
{
	for (auto& it : vec) {
		it = 0.355768 - 0.487396 * cos(m_phase) +
			0.144232 * cos(2.0 * m_phase) -
			0.012604 * cos(3.0 * m_phase);
		m_phase += m_step;
	}
}

template <class Type>
inline void Window<Type>::flattop(vector<Type>& vec)
{
	for (auto& it : vec) {
		it = 1.0 - 1.93 * cos(m_phase) +
			1.29 * cos(2.0 * m_phase) -
			0.388 * cos(3.0 * m_phase) +
			0.032 * cos(4.0 * m_phase);
		m_phase += m_step;
	}
}

template <class Type>
inline void Window<Type>::proces(vector<Type>& vec)
{
	vec *= m_vector;
}

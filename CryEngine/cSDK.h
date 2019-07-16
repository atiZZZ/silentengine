#pragma once
#define DBL_MIN          2.2250738585072014e-308
#define ILINE __forceinline
#define BIT(x)   (1 << (x))

typedef unsigned int EntityId;
typedef unsigned int uint32;
typedef signed int int32;
typedef float  f32;

enum type_zero { ZERO };
enum type_min { VMIN };
enum type_max { VMAX };
enum type_identity { IDENTITY };

ILINE float _sqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;
	x2 = number * 0.5F;
	y = number;
	i = *(long*)& y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)& i;
	y = y * (threehalfs - (x2 * y * y));
	y = y * (threehalfs - (x2 * y * y));
	y = y * (threehalfs - (x2 * y * y));
	return 1 / y;
}

ILINE float isqrt_safe_tpl(float op) { return 1.0f / (float)_sqrt(op + (float)DBL_MIN); }
ILINE int32 int_round(f32 f) { return f < 0.f ? int32(f - 0.5f) : int32(f + 0.5f); }
//ILINE f32   fabs_tpl(f32 op) { return fabsf(op); }
//ILINE int32 fabs_tpl(int32 op) { int32 mask = op >> 31; return op + mask ^ mask; }

template <typename F> struct Vec3_tpl {
	F x, y, z;
	ILINE Vec3_tpl(type_zero) : x(0), y(0), z(0) {}
	ILINE Vec3_tpl(type_min);
	ILINE Vec3_tpl(type_max);
	explicit ILINE Vec3_tpl(F f) : x(f), y(f), z(f) { }
	ILINE Vec3_tpl() {
		Vec3_tpl(0, 0, 0);
	}
	ILINE Vec3_tpl(F xt, F yt, F zt) {
		x = xt;
		y = yt;
		z = zt;
	}
	ILINE F length() const { return (F)_sqrt(x * x + y * y + z * z); }
	template<typename T> ILINE F sum(T a) {
		F s = 0;
		for (int i = 0; i < a.size; ++i) s += a[i];
		return s;
	}
	template<typename T> ILINE F dot(T a, T b) { return sum(a * b); }

	ILINE F  distance(Vec3_tpl VecB) { return (F)_sqrt(((x - VecB.x) * (x - VecB.x)) + ((y - VecB.y) * (y - VecB.y)) + ((z - VecB.z) * (z - VecB.z))); }

	ILINE F & operator[](int index) { return ((F*)this)[index]; }
	ILINE F  operator[](int index) const { return ((F*)this)[index]; }

	void operator += (F f) { x += f; y += f; z += f; }
	void operator -= (F f) { x -= f; y -= f; z -= f; }
	ILINE Vec3_tpl<F> operator*(F k) const {
		const Vec3_tpl<F> v = *this;
		return Vec3_tpl<F>(v.x * k, v.y * k, v.z * k);
	}
	ILINE Vec3_tpl& operator /  (F f) { return Vec3_tpl(x / f, y / f, z / f); }
	ILINE Vec3_tpl& operator = (const Vec3_tpl & source) { x = source.x; y = source.y; z = source.z; return *this; }
	ILINE Vec3_tpl& operator = (const F value) { x = value; y = value; z = value;   return *this; }
//	ILINE Vec3_tpl& operator * (F f) { return Vec3_tpl(x * f, y * f, z * f); }
	ILINE Vec3_tpl& operator *= (F f) { x *= f; y *= f; z *= f; return *this; }
	ILINE Vec3_tpl& operator /= (F f) { x /= f; y /= f; z /= f; return *this; }

	ILINE friend Vec3_tpl operator + (const Vec3_tpl & vec1, const Vec3_tpl & vec2) { return Vec3_tpl(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z); }
	ILINE friend Vec3_tpl operator + (const Vec3_tpl & vec1, const F * vec2) { return vec1 + Vec3_tpl(vec2); }
	ILINE friend Vec3_tpl operator + (const F * vec1, const Vec3_tpl & vec2) { return Vec3_tpl(vec1) + vec2; }

	ILINE friend Vec3_tpl operator - (const Vec3_tpl & vec1, const Vec3_tpl & vec2) { return Vec3_tpl(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z); }
	ILINE friend Vec3_tpl operator - (const Vec3_tpl & vec1, const F * vec2) { return vec1 - Vec3_tpl(vec2); }
	ILINE friend Vec3_tpl operator - (const F * vec1, const Vec3_tpl & vec2) { return Vec3_tpl(vec1) - vec2; }

	ILINE Vec3_tpl<F>& Set(const F xval, const F yval, const F zval) {
		x = xval; y = yval; z = zval;
		return *this;
	}

	ILINE Vec3_tpl& normalize() {
		F len2 = x * x + y * y + z * z;
		if (len2 > (F)1e-20f) {
			F rlen = _sqrt(len2);
			x *= rlen;
			y *= rlen;
			z *= rlen;
		}
		else Set(0, 0, 1);

		return *this;
	}
	ILINE Vec3_tpl<F> GetNormalized() const {
		FLOAT fInvLen = (FLOAT)isqrt_safe_tpl(x * x + y * y + z * z);
		Vec3_tpl<F> out = *this;
		out.x *= fInvLen;
		out.y *= fInvLen;
		out.z *= fInvLen;
		return out;
	}
};
template <typename F> struct Vec4_tpl {
	F x, y, z, w;
	ILINE Vec4_tpl(type_zero) : x(0), y(0), z(0), w(0) {}
	ILINE Vec4_tpl(type_min);
	ILINE Vec4_tpl(type_max);
	explicit ILINE Vec4_tpl(F f) : x(f), y(f), z(f), w(f) { }
	ILINE Vec4_tpl() {
		Vec4_tpl(0, 0, 0, 0);
	}
	ILINE Vec4_tpl(F xt, F yt, F zt, F wt) {
		x = xt;
		y = yt;
		z = zt;
		w = wt;
	}
};
template <typename F> struct Quat_tpl {
	Vec3_tpl<F> v;
	F w;
	ILINE  Quat_tpl() {}
	ILINE  Quat_tpl(F angle, const Vec3_tpl<F>& axis) : w(angle), v(axis) {};
	ILINE  Quat_tpl(type_identity) : w(1), v(0, 0, 0) {}

	ILINE  bool operator!=(const Quat_tpl<F>& q) const { return !(*this == q); }
	ILINE  Quat_tpl<F>	operator - () const { return Quat_tpl<F>(-w, -v); };

	/*ILINE  bool IsEquivalent(const Quat_tpl<F>& q, F e = VEC_EPSILON) const {
		Quat_tpl<F> p = -q;
		bool t0 = (fabs_tpl(v.x - q.v.x) <= e) && (fabs_tpl(v.y - q.v.y) <= e) && (fabs_tpl(v.z - q.v.z) <= e) && (fabs_tpl(w - q.w) <= e);
		bool t1 = (fabs_tpl(v.x - p.v.x) <= e) && (fabs_tpl(v.y - p.v.y) <= e) && (fabs_tpl(v.z - p.v.z) <= e) && (fabs_tpl(w - p.w) <= e);
		t0 |= t1;
		return t0;
	}*/
	ILINE   void  SetRotationVDir(const Vec3_tpl<F> & vdir) {
		w = (0.70710676908493042f);
		v.x = (vdir.z * 0.70710676908493042f);
		v.y = (0.0f);
		v.z = (0.0f);

		F l = (F)_sqrt(vdir.x * vdir.x + vdir.y * vdir.y);

		if (l > (0.00001)) {
			Vec3_tpl<F> hv;
			hv.x = vdir.x / l;
			hv.y = vdir.y / l + 1.0f;
			hv.z = l + 1.0f;

			F r = (F)_sqrt(hv.x * hv.x + hv.y * hv.y);
			F s = (F)_sqrt(hv.z * hv.z + vdir.z * vdir.z);
			F hacos0 = 0.0;
			F hasin0 = -1.0;

			if (r > (0.00001)) {
				hacos0 = hv.y / r;
				hasin0 = -hv.x / r;
			}
			F hacos1 = hv.z / s;
			F hasin1 = vdir.z / s;
			w = (hacos0 * hacos1);
			v.x = (hacos0 * hasin1);
			v.y = (hasin0 * hasin1);
			v.z = (hasin0 * hacos1);
		}
	}
	static Quat_tpl<F> CreateRotationVDir(const Vec3_tpl<F> & vdir) {
		Quat_tpl<F> q;
		q.SetRotationVDir(vdir);
		return q;
	}
};
template <typename F> struct QuatT_tpl {
	Quat_tpl<F>   q;
	Vec3_tpl<F> t;
	ILINE  QuatT_tpl() {};
	ILINE  QuatT_tpl(const Vec3_tpl<F>& _t, const Quat_tpl<F>& _q) { q = _q; t = _t; }
	ILINE  QuatT_tpl(type_identity) { q.w = 1; q.v.x = 0; q.v.y = 0; q.v.z = 0; t.x = 0; t.y = 0; t.z = 0; }

};
template <typename F> struct Matrix33_tpl {
	F m00, m01, m02;
	F m10, m11, m12;
	F m20, m21, m22;
	ILINE  Matrix33_tpl() {};
	explicit ILINE Matrix33_tpl<F>(const Quat_tpl<F>& q) {
		Vec3_tpl<F> v2 = q.v + q.v;
		F xx = 1 - v2.x * q.v.x;
		F yy = v2.y * q.v.y;
		F xw = v2.x * q.w;
		F xy = v2.y * q.v.x;
		F yz = v2.z * q.v.y;
		F yw = v2.y * q.w;
		F xz = v2.z * q.v.x;
		F zz = v2.z * q.v.z;
		F zw = v2.z * q.w;
		m00 = 1 - yy - zz;
		m01 = xy - zw;
		m02 = xz + yw;
		m10 = xy + zw;
		m11 = xx - zz;
		m12 = yz - xw;
		m20 = xz - yw;
		m21 = yz + xw;
		m22 = xx - yy;
	}
	template<typename F1> ILINE  Matrix33_tpl(const Matrix33_tpl<F1>& m) {
		m00 = F(m.m00);	m01 = F(m.m01);	m02 = F(m.m02);
		m10 = F(m.m10);	m11 = F(m.m11);	m12 = F(m.m12);
		m20 = F(m.m20);	m21 = F(m.m21);	m22 = F(m.m22);
	}
	ILINE  Matrix33_tpl& operator = (const Matrix33_tpl<F>& m) {
		m00 = m.m00;	m01 = m.m01;	m02 = m.m02;
		m10 = m.m10;	m11 = m.m11;	m12 = m.m12;
		m20 = m.m20;	m21 = m.m21;	m22 = m.m22;
		return *this;
	}
};
template <typename F> struct Matrix34_tpl {
	F m00, m01, m02, m03;
	F m10, m11, m12, m13;
	F m20, m21, m22, m23;

	ILINE Matrix34_tpl() {};
	template<class F1> explicit Matrix34_tpl(const QuatT_tpl<F1>& q) {
		Vec3_tpl<F1> v2 = q.q.v + q.q.v;
		F1 xx = 1 - v2.x * q.q.v.x;		F1 yy = v2.y * q.q.v.y;		F1 xw = v2.x * q.q.w;
		F1 xy = v2.y * q.q.v.x;			F1 yz = v2.z * q.q.v.y;		F1 yw = v2.y * q.q.w;
		F1 xz = v2.z * q.q.v.x;			F1 zz = v2.z * q.q.v.z;		F1 zw = v2.z * q.q.w;
		m00 = F(1 - yy - zz);			m01 = F(xy - zw);			m02 = F(xz + yw);			m03 = F(q.t.x);
		m10 = F(xy + zw);				m11 = F(xx - zz);			m12 = F(yz - xw);			m13 = F(q.t.y);
		m20 = F(xz - yw);				m21 = F(yz + xw);			m22 = F(xx - yy);			m23 = F(q.t.z);
	}
	ILINE Vec3_tpl <F>GetTranslation() const { return Vec3_tpl <F>(m03, m13, m23); }

	ILINE Matrix34_tpl <F>SetTranslation(Vec3_tpl<F>vPos) {
		m03 = F(vPos.x);
		m13 = F(vPos.y);
		m23 = F(vPos.z);
		return *this;
	}
};
template<typename F> struct Matrix44_tpl {
	F m00, m01, m02, m03;
	F m10, m11, m12, m13;
	F m20, m21, m22, m23;
	F m30, m31, m32, m33;

	ILINE Matrix44_tpl() {};
	ILINE void SetZero() {
		m00 = 0;
		m01 = 0;
		m02 = 0;
		m03 = 0;
		m10 = 0;
		m11 = 0;
		m12 = 0;
		m13 = 0;
		m20 = 0;
		m21 = 0;
		m22 = 0;
		m23 = 0;
		m30 = 0;
		m31 = 0;
		m32 = 0;
		m33 = 0;
	}
	ILINE void SetIdentity()
	{
		m00 = 1;
		m01 = 0;
		m02 = 0;
		m03 = 0;
		m10 = 0;
		m11 = 1;
		m12 = 0;
		m13 = 0;
		m20 = 0;
		m21 = 0;
		m22 = 1;
		m23 = 0;
		m30 = 0;
		m31 = 0;
		m32 = 0;
		m33 = 1;
	}
	ILINE Matrix44_tpl(type_zero) { SetZero(); }
	ILINE F& operator[](int index) { return ((F*)this)[index]; }
	ILINE F  operator[](int index) const { return ((F*)this)[index]; }
};

template<class F1, class F2>ILINE Vec3_tpl<F1> operator*(const Matrix33_tpl<F2>& m, const Vec3_tpl<F1>& p) {
	Vec3_tpl<F1> tp;
	tp.x = F1(m.m00 * p.x + m.m01 * p.y + m.m02 * p.z);
	tp.y = F1(m.m10 * p.x + m.m11 * p.y + m.m12 * p.z);
	tp.z = F1(m.m20 * p.x + m.m21 * p.y + m.m22 * p.z);
	return tp;
}
template<class F, class F2> Vec3_tpl<F> operator*(const QuatT_tpl<F> & q, const Vec3_tpl<F2> & v) {
	Vec3_tpl<F> out, r2;
	r2.x = (q.q.v.y * v.z - q.q.v.z * v.y) + q.q.w * v.x;
	r2.y = (q.q.v.z * v.x - q.q.v.x * v.z) + q.q.w * v.y;
	r2.z = (q.q.v.x * v.y - q.q.v.y * v.x) + q.q.w * v.z;
	out.x = (r2.z * q.q.v.y - r2.y * q.q.v.z);
	out.x += out.x + v.x + q.t.x;
	out.y = (r2.x * q.q.v.z - r2.z * q.q.v.x);
	out.y += out.y + v.y + q.t.y;
	out.z = (r2.y * q.q.v.x - r2.x * q.q.v.y);
	out.z += out.z + v.z + q.t.z;
	return out;
}
template<class F> Vec3_tpl<F> operator*(const Matrix34_tpl<F> & m, const Vec3_tpl<F> & p) {
	Vec3_tpl<F> tp;
	tp.x = m.m00 * p.x + m.m01 * p.y + m.m02 * p.z + m.m03;
	tp.y = m.m10 * p.x + m.m11 * p.y + m.m12 * p.z + m.m13;
	tp.z = m.m20 * p.x + m.m21 * p.y + m.m22 * p.z + m.m23;
	return tp;
}
template <class F> Matrix34_tpl<F> operator * (const Matrix34_tpl<F> & l, const Matrix34_tpl<F> & r) {
	Matrix34_tpl<F> m;
	m.m00 = l.m00 * r.m00 + l.m01 * r.m10 + l.m02 * r.m20;
	m.m10 = l.m10 * r.m00 + l.m11 * r.m10 + l.m12 * r.m20;
	m.m20 = l.m20 * r.m00 + l.m21 * r.m10 + l.m22 * r.m20;
	m.m01 = l.m00 * r.m01 + l.m01 * r.m11 + l.m02 * r.m21;
	m.m11 = l.m10 * r.m01 + l.m11 * r.m11 + l.m12 * r.m21;
	m.m21 = l.m20 * r.m01 + l.m21 * r.m11 + l.m22 * r.m21;
	m.m02 = l.m00 * r.m02 + l.m01 * r.m12 + l.m02 * r.m22;
	m.m12 = l.m10 * r.m02 + l.m11 * r.m12 + l.m12 * r.m22;
	m.m22 = l.m20 * r.m02 + l.m21 * r.m12 + l.m22 * r.m22;
	m.m03 = l.m00 * r.m03 + l.m01 * r.m13 + l.m02 * r.m23 + l.m03;
	m.m13 = l.m10 * r.m03 + l.m11 * r.m13 + l.m12 * r.m23 + l.m13;
	m.m23 = l.m20 * r.m03 + l.m21 * r.m13 + l.m22 * r.m23 + l.m23;
	return m;
}

template<class F> Vec3_tpl<F> operator*(const Vec3_tpl<F> & m, const Vec3_tpl<F> & p) {
	return Vec3_tpl<F>(m.x * p.x, m.y * p.y, m.z * p.z);
}
typedef Vec3_tpl<float>			Vec3;
typedef Vec4_tpl<float>			Vec4;
typedef Quat_tpl<float>			Quat;
typedef QuatT_tpl<float>		QuatT;
typedef Quat_tpl<float>			quaternionf;
typedef Matrix33_tpl<float>		Matrix33;
typedef Matrix34_tpl<float>		Matrix34;
typedef Matrix44_tpl<FLOAT>		Matrix44;

struct AABB {
	Vec3 min;
	Vec3 max;
	ILINE Vec3 GetCenter() const { return (min + max) * 0.5f; }
	ILINE bool IsReset() const { return min.x > max.x; }
	ILINE void Reset() { min = Vec3(1e15f);  max = Vec3(-1e15f); }
	/*ILINE void SetTransformedAABB(const QuatT& qt, const AABB& aabb) {
		if (aabb.IsReset())Reset();
		else {
			Matrix33 m33 = Matrix33(qt.q);
			m33.m00 = (float)fabsf(m33.m00);
			m33.m01 = (float)fabsf(m33.m01);
			m33.m02 = (float)fabsf(m33.m02);
			m33.m10 = (float)fabsf(m33.m10);
			m33.m11 = (float)fabsf(m33.m11);
			m33.m12 = (float)fabsf(m33.m12);
			m33.m20 = (float)fabsf(m33.m20);
			m33.m21 = (float)fabsf(m33.m21);
			m33.m22 = (float)fabsf(m33.m22);
			Vec3 sz = m33 * ((aabb.max - aabb.min) * 0.5f);
			Vec3 pos = qt * ((aabb.max + aabb.min) * 0.5f);
			min = pos - sz;
			max = pos + sz;
		}
	}
	ILINE void SetTransformedAABB(const Matrix34& m34, const AABB& aabb) {
		if (aabb.IsReset())Reset();
		else {
			Matrix33 m33;
			m33.m00 = (float)fabsf(m34.m00);
			m33.m01 = (float)fabsf(m34.m01);
			m33.m02 = (float)fabsf(m34.m02);
			m33.m10 = (float)fabsf(m34.m10);
			m33.m11 = (float)fabsf(m34.m11);
			m33.m12 = (float)fabsf(m34.m12);
			m33.m20 = (float)fabsf(m34.m20);
			m33.m21 = (float)fabsf(m34.m21);
			m33.m22 = (float)fabsf(m34.m22);

			Vec3 sz = m33 * ((aabb.max - aabb.min) * 0.5f);
			Vec3 pos = m34 * ((aabb.max + aabb.min) * 0.5f);
			min = pos - sz;
			max = pos + sz;
		}
	}
	ILINE static AABB CreateTransformedAABB(const Matrix34& m34, const AABB& aabb) {
		AABB taabb; taabb.SetTransformedAABB(m34, aabb); return taabb;
	}*/
};




enum KeyCodes :INT {

	BACKSPACE = 8,
	TAB = 9,
	ENTER = 13,
	SHIFT = 16,
	CTRL = 17,
	ALT = 18,
	PAUSE_BREAK = 19,
	CAPS_LOCK = 20,
	ESCAPE = 27,
	PAGE_UP = 33,
	PAGE_DOWN = 34,
	END = 35,
	HOME = 36,
	LEFT_ARROW = 37,
	UP_ARROW = 38,
	RIGHT_ARROW = 39,
	DOWN_ARROW = 40,
	INSERT = 45,
	DELETE_ = 46,
	_0 = 48,
	_1 = 49,
	_2 = 50,
	_3 = 51,
	_4 = 52,
	_5 = 53,
	_6 = 54,
	_7 = 55,
	_8 = 56,
	_9 = 57,
	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,
	LEFT_WINDOW = 91,
	RIGHT_WINDOW = 92,
	SELECT_KEY = 93,
	NUMPAD_0 = 96,
	NUMPAD_1 = 97,
	NUMPAD_2 = 98,
	NUMPAD_3 = 99,
	NUMPAD_4 = 100,
	NUMPAD_5 = 101,
	NUMPAD_6 = 102,
	NUMPAD_7 = 103,
	NUMPAD_8 = 104,
	NUMPAD_9 = 105,
	MULTIPLY = 106,
	ADD = 107,
	SUBTRACT = 109,
	DECIMAL_POINT = 110,
	DIVIDE = 111,
	F1 = 112,
	F2 = 113,
	F3 = 114,
	F4 = 115,
	F5 = 116,
	F6 = 117,
	F7 = 118,
	F8 = 119,
	F9 = 120,
	F10 = 121,
	F11 = 122,
	F12 = 123,
	NUM_LOCK = 144,
	SCROLL_LOCK = 145,
	SEMI_COLON = 186,
	EQUAL_SIGN = 187,
	COMMA = 188,
	DASH = 189,
	PERIOD = 190,
	FORWARD_SLASH = 191,
	GRAVE_ACCENT = 192,
	OPEN_BRACKET = 219,
	BACK_SLASH = 220,
	CLOSE_BRAKET = 221,
	SINGLE_QUOTE = 222
};

enum EEntityProxy {
	ENTITY_PROXY_RENDER,
	ENTITY_PROXY_PHYSICS,
	ENTITY_PROXY_SCRIPT,
	ENTITY_PROXY_AUDIO,
	ENTITY_PROXY_AI,
	ENTITY_PROXY_AREA,
	ENTITY_PROXY_BOIDS,
	ENTITY_PROXY_BOID_OBJECT,
	ENTITY_PROXY_CAMERA,
	ENTITY_PROXY_FLOWGRAPH,
	ENTITY_PROXY_SUBSTITUTION,
	ENTITY_PROXY_TRIGGER,
	ENTITY_PROXY_ROPE,
	ENTITY_PROXY_ENTITYNODE,
	ENTITY_PROXY_ATTRIBUTES,
	ENTITY_PROXY_CLIPVOLUME,
	ENTITY_PROXY_DYNAMICRESPONSE,
	ENTITY_PROXY_USER,
	ENTITY_PROXY_LAST
};
enum EEntityXFormFlags {
	ENTITY_XFORM_POS = BIT(1),
	ENTITY_XFORM_ROT = BIT(2),
	ENTITY_XFORM_SCL = BIT(3),
	ENTITY_XFORM_NO_PROPOGATE = BIT(4),
	ENTITY_XFORM_FROM_PARENT = BIT(5),   //!< When parent changes his transformation.
	ENTITY_XFORM_PHYSICS_STEP = BIT(13),
	ENTITY_XFORM_EDITOR = BIT(14),
	ENTITY_XFORM_TRACKVIEW = BIT(15),
	ENTITY_XFORM_TIMEDEMO = BIT(16),
	ENTITY_XFORM_NOT_REREGISTER = BIT(17),  //!< Optimization flag, when set object will not be re-registered in 3D engine.
	ENTITY_XFORM_NO_EVENT = BIT(18),  //!< Suppresses ENTITY_EVENT_XFORM event.
	ENTITY_XFORM_NO_SEND_TO_ENTITY_SYSTEM = BIT(19),
	ENTITY_XFORM_USER = 0x1000000,
};
enum EEntityGetSetSlotFlags {
	ENTITY_SLOT_ACTUAL = 1 << 31
};
enum ESilhouettesParams {
	eAllMap = 524312,
	eLimitDistance = 524296
};

enum EEntityFlags
{
	//////////////////////////////////////////////////////////////////////////
	// Persistent flags (can be set from the editor).
	ENTITY_FLAG_CASTSHADOW = BIT(1),
	ENTITY_FLAG_UNREMOVABLE = BIT(2),        //!< This entity cannot be removed using IEntitySystem::RemoveEntity until this flag is cleared.
	ENTITY_FLAG_GOOD_OCCLUDER = BIT(3),
	ENTITY_FLAG_NO_DECALNODE_DECALS = BIT(4),
	//////////////////////////////////////////////////////////////////////////

	ENTITY_FLAG_WRITE_ONLY = BIT(5),
	ENTITY_FLAG_NOT_REGISTER_IN_SECTORS = BIT(6),
	ENTITY_FLAG_CALC_PHYSICS = BIT(7),
	ENTITY_FLAG_CLIENT_ONLY = BIT(8),
	ENTITY_FLAG_SERVER_ONLY = BIT(9),
	ENTITY_FLAG_CUSTOM_VIEWDIST_RATIO = BIT(10),  //!< This entity have special custom view distance ratio (AI/Vehicles must have it).
	ENTITY_FLAG_CALCBBOX_USEALL = BIT(11),  //!< use character and objects in BBOx calculations.
	ENTITY_FLAG_VOLUME_SOUND = BIT(12),  //!< Entity is a volume sound (will get moved around by the sound proxy).
	ENTITY_FLAG_HAS_AI = BIT(13),  //!< Entity has an AI object.
	ENTITY_FLAG_TRIGGER_AREAS = BIT(14),  //!< This entity will trigger areas when it enters them.
	ENTITY_FLAG_NO_SAVE = BIT(15),  //!< This entity will not be saved.
	ENTITY_FLAG_CAMERA_SOURCE = BIT(16),  //!< This entity is a camera source.
	ENTITY_FLAG_CLIENTSIDE_STATE = BIT(17),  //!< Prevents error when state changes on the client and does not sync state changes to the client.
	ENTITY_FLAG_SEND_RENDER_EVENT = BIT(18),  //!< When set entity will send ENTITY_EVENT_RENDER every time its rendered.
	ENTITY_FLAG_NO_PROXIMITY = BIT(19),  //!< Entity will not be registered in the partition grid and can not be found by proximity queries.
	ENTITY_FLAG_PROCEDURAL = BIT(20),  //!< Entity has been generated at runtime.
	ENTITY_FLAG_UPDATE_HIDDEN = BIT(21),  //!< Entity will be update even when hidden.
	ENTITY_FLAG_NEVER_NETWORK_STATIC = BIT(22),  //!< Entity should never be considered a static entity by the network system.
	ENTITY_FLAG_IGNORE_PHYSICS_UPDATE = BIT(23),  //!< Used by Editor only, (don't set).
	ENTITY_FLAG_SPAWNED = BIT(24),  //!< Entity was spawned dynamically without a class.
	ENTITY_FLAG_SLOTS_CHANGED = BIT(25),  //!< Entity's slots were changed dynamically.
	ENTITY_FLAG_MODIFIED_BY_PHYSICS = BIT(26),  //!< Entity was procedurally modified by physics.
	ENTITY_FLAG_OUTDOORONLY = BIT(27),  //!< Same as Brush->Outdoor only.
	ENTITY_FLAG_SEND_NOT_SEEN_TIMEOUT = BIT(28),  //!< Entity will be sent ENTITY_EVENT_NOT_SEEN_TIMEOUT if it is not rendered for 30 seconds.
	ENTITY_FLAG_RECVWIND = BIT(29),  //!< Receives wind.
	ENTITY_FLAG_LOCAL_PLAYER = BIT(30),
	ENTITY_FLAG_AI_HIDEABLE = BIT(31),  //!< AI can use the object to calculate automatic hide points.
};


enum EPE_Params
{
	ePE_params_pos = 0,
	ePE_player_dimensions = 1,
	ePE_params_car = 2,
	ePE_params_particle = 3,
	ePE_player_dynamics = 4,
	ePE_params_joint = 5,
	ePE_params_part = 6,
	ePE_params_sensors = 7,
	ePE_params_articulated_body = 8,
	ePE_params_outer_entity = 9,
	ePE_simulation_params = 10,
	ePE_params_foreign_data = 11,
	ePE_params_buoyancy = 12,
	ePE_params_rope = 13,
	ePE_params_bbox = 14,
	ePE_params_flags = 15,
	ePE_params_wheel = 16,
	ePE_params_softbody = 17,
	ePE_params_area = 18,
	ePE_tetrlattice_params = 19,
	ePE_params_ground_plane = 20,
	ePE_params_structural_joint = 21,
	ePE_params_waterman = 22,
	ePE_params_timeout = 23,
	ePE_params_skeleton = 24,
	ePE_params_structural_initial_velocity = 25,
	ePE_params_collision_class = 26,

	ePE_Params_Count
};

enum EActorSpectatorMode
{
	eASM_None = 0,												// normal, non-spectating

	eASM_FirstMPMode,
	eASM_Fixed = eASM_FirstMPMode,				// fixed position camera
	eASM_Free,														// free roaming, no collisions
	eASM_Follow,													// follows an entity in 3rd person
	eASM_Killer,													// Front view of the killer in 3rdperson.
	eASM_LastMPMode = eASM_Killer,

	eASM_Cutscene,												// HUDInterfaceEffects.cpp sets this
};

enum phentity_flags
{
	particle_single_contact = 0x01,  //!< Full stop after first contact.
	particle_constant_orientation = 0x02,  //!< Forces constant orientation.
	particle_no_roll = 0x04,  //!< 'sliding' mode; entity's 'normal' vector axis will be alinged with the ground normal.
	particle_no_path_alignment = 0x08,  //!< Unless set, entity's y axis will be aligned along the movement trajectory.
	particle_no_spin = 0x10,  //!< Disables spinning while flying.
	particle_no_self_collisions = 0x100, //!< Disables collisions with other particles.
	particle_no_impulse = 0x200, //!< Particle will not add hit impulse (expecting that some other system will).

	lef_push_objects = 0x01, lef_push_players = 0x02, //!< Push objects and players during contacts.
	lef_snap_velocities = 0x04,                          //!< Quantizes velocities after each step (was ised in MP for precise deterministic sync).
	lef_loosen_stuck_checks = 0x08,                          //!< Don't do additional intersection checks after each step (recommended for NPCs to improve performance).
	lef_report_sliding_contacts = 0x10,                          //!< Unless set, 'grazing' contacts are not reported.

	rope_findiff_attached_vel = 0x01,                              //!< Approximate velocity of the parent object as v = (pos1-pos0)/time_interval.
	rope_no_solver = 0x02,                              //!< No velocity solver; will rely on stiffness (if set) and positional length enforcement.
	rope_ignore_attachments = 0x4,                               //!< No collisions with objects the rope is attached to.
	rope_target_vtx_rel0 = 0x08, rope_target_vtx_rel1 = 0x10, //!< Whether target vertices are set in the parent entity's frame.
	rope_subdivide_segs = 0x100,                             //!< Turns on 'dynamic subdivision' mode (only in this mode contacts in a strained state are handled correctly).
	rope_no_tears = 0x200,                             //!< Rope will not tear when it reaches its force limit, but stretch.
	rope_collides = 0x200000,                          //!< Rope will collide with objects other than the terrain.
	rope_collides_with_terrain = 0x400000,                          //!< Rope will collide with the terrain.
	rope_collides_with_attachment = 0x80,                              //!< Rope will collide with the objects it's attached to even if the other collision flags are not set.
	rope_no_stiffness_when_colliding = 0x10000000,                        //!< Rope will use stiffness 0 if it has contacts.
	se_skip_longest_edges = 0x01, //!< the longest edge in each triangle with not participate in the solver
	se_rigid_core = 0x02, //!< soft body will have an additional rigid body core

	ref_use_simple_solver = 0x01,                                //!< use penalty-based solver (obsolete)
	ref_no_splashes = 0x04,                                //!< will not generate EventPhysCollisions when contacting water
	ref_checksum_received = 0x04, ref_checksum_outofsync = 0x08, //!< obsolete
	ref_small_and_fast = 0x100,                               //!< entity will trace rays against alive characters; set internally unless overriden																																									
	aef_recorded_physics = 0x02, //!< specifies a an entity that contains pre-baked physics simulation
	wwef_fake_inner_wheels = 0x08, //!< exclude wheels between the first and the last one from the solver
	pef_parts_traceable = 0x10,                                                      //!< each entity part will be registered separately in the entity grid
	pef_disabled = 0x20,                                                      //!< entity will not be simulated
	pef_never_break = 0x40,                                                      //!< entity will not break or deform other objects
	pef_deforming = 0x80,                                                      //!< entity undergoes a dynamic breaking/deforming
	pef_pushable_by_players = 0x200,                                                     //!< entity can be pushed by playerd
	pef_traceable = 0x400, particle_traceable = 0x400, rope_traceable = 0x400, //!< entity is registered in the entity grid
	pef_update = 0x800,                                                     //!< only entities with this flag are updated if ent_flagged_only is used in TimeStep()
	pef_monitor_state_changes = 0x1000,                                                    //!< generate immediate events for simulation class changed (typically rigid bodies falling asleep)
	pef_monitor_collisions = 0x2000,                                                    //!< generate immediate events for collisions
	pef_monitor_env_changes = 0x4000,                                                    //!< generate immediate events when something breaks nearby
	pef_never_affect_triggers = 0x8000,                                                    //!< don't generate events when moving through triggers
	pef_invisible = 0x10000,                                                   //!< will apply certain optimizations for invisible entities
	pef_ignore_ocean = 0x20000,                                                   //!< entity will ignore global water area
	pef_fixed_damping = 0x40000,                                                   //!< entity will force its damping onto the entire group
	pef_monitor_poststep = 0x80000,                                                   //!< entity will generate immediate post step events
	pef_always_notify_on_deletion = 0x100000,                                                  //!< when deleted, entity will awake objects around it even if it's not referenced (has refcount 0)
	pef_override_impulse_scale = 0x200000,                                                  //!< entity will ignore breakImpulseScale in PhysVars
	pef_players_can_break = 0x400000,                                                  //!< playes can break the entiy by bumping into it
	pef_cannot_squash_players = 0x10000000,                                                //!< entity will never trigger 'squashed' state when colliding with players
	pef_ignore_areas = 0x800000,                                                  //!< entity will ignore phys areas (gravity and water)
	pef_log_state_changes = 0x1000000,                                                 //!< entity will log simulation class change events
	pef_log_collisions = 0x2000000,                                                 //!< entity will log collision events
	pef_log_env_changes = 0x4000000,                                                 //!< entity will log EventPhysEnvChange when something breaks nearby
	pef_log_poststep = 0x8000000,                                                 //!< entity will log EventPhysPostStep events
};

enum entity_query_flags
{
	ent_static = 1, ent_sleeping_rigid = 2, ent_rigid = 4, ent_living = 8, ent_independent = 16, ent_deleted = 128, ent_terrain = 0x100,
	ent_all = ent_static | ent_sleeping_rigid | ent_rigid | ent_living | ent_independent | ent_terrain,
	ent_flagged_only = pef_update, ent_skip_flagged = pef_update * 2, //!< "flagged" meas has pef_update set
	ent_areas = 32, ent_triggers = 64,
	ent_ignore_noncolliding = 0x10000,
	ent_sort_by_mass = 0x20000,  //!< sort by mass in ascending order
	ent_allocate_list = 0x40000,  //!< if not set, the function will return an internal pointer
	ent_addref_results = 0x100000, //!< will call AddRef on each entity in the list (expecting the caller call Release)
	ent_water = 0x200,    //!< can only be used in RayWorldIntersection
	ent_no_ondemand_activation = 0x80000,  //!< can only be used in RayWorldIntersection
	ent_delayed_deformations = 0x80000   //!< queues procedural breakage requests; can only be used in SimulateExplosion
};
class cVmtHook
{
public:
	void bInitialize(PDWORD* ppdwClassBase)
	{
		m_ppdwClassBase = ppdwClassBase;
		m_pdwOldVMT = *ppdwClassBase;
		m_dwVMTSize = dwGetVMTCount(*ppdwClassBase);
		m_pdwNewVMT = new DWORD[m_dwVMTSize];
		memcpy(m_pdwNewVMT, m_pdwOldVMT, sizeof(DWORD) * m_dwVMTSize);
		*ppdwClassBase = m_pdwNewVMT;
	}
	void ReLoad(PDWORD* ppdwClassBase)
	{
		m_ppdwClassBase = ppdwClassBase;
		m_pdwOldVMT = *ppdwClassBase;
		*ppdwClassBase = m_pdwNewVMT;
	}
	void bInitialize(PDWORD** pppdwClassBase)
	{
		bInitialize(*pppdwClassBase);
	}
	void UnHook(void)
	{
		if (m_ppdwClassBase) * m_ppdwClassBase = m_pdwOldVMT;
	}
	void ReHook(void)
	{
		if (m_ppdwClassBase) * m_ppdwClassBase = m_pdwNewVMT;
	}
	int iGetFuncCount(void)
	{
		return (int)m_dwVMTSize;
	}
	DWORD dwGetMethodAddress(int Index)
	{
		if (Index >= 0 && Index <= (int)m_dwVMTSize && m_pdwOldVMT != NULL)
			return m_pdwOldVMT[Index];
		return NULL;
	}
	PDWORD pdwGetOldVMT(void) { return m_pdwOldVMT; }
	PDWORD pdwGetNewVMT(void) { return m_pdwNewVMT; }
	DWORD dwHookMethod(DWORD dwNewFunc, unsigned int iIndex)
	{
		if (m_pdwNewVMT&& m_pdwOldVMT&& iIndex <= m_dwVMTSize && iIndex >= 0)
		{
			m_pdwNewVMT[iIndex] = dwNewFunc;
			return m_pdwOldVMT[iIndex];
		}
		return NULL;
	}
	template <class cData> cData GetOrigFuntion(UINT index)
	{
		return (cData)m_pdwOldVMT[index];
	}
private:
	DWORD dwGetVMTCount(PDWORD pdwVMT)
	{
		DWORD dwIndex = 0;
		for (dwIndex = 0; pdwVMT[dwIndex]; dwIndex++)
			if (IsBadCodePtr((FARPROC)pdwVMT[dwIndex]))
				break;
		return dwIndex;
	}
	PDWORD * m_ppdwClassBase;
	PDWORD	m_pdwNewVMT, m_pdwOldVMT;
	DWORD	m_dwVMTSize;
};
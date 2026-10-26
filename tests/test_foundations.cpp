#include "Test.h"
#include "../content/contest/Base.h"
#include "../content/geometry/Point3D.h"
#define main notebook_main
#include "../content/contest/Template.h"
#undef main

void test_base() {
	CHECK(sizeof(i64) == 8 && sizeof(u64) == 8);
	CHECK(sizeof(i128) == 16 && sizeof(u128) == 16);
	i128 x = i128(1) << 100;
	CHECK(x * 3 / 3 == x && -x < 0);
	u128 y = u128(1) << 127;
	CHECK(y / 2 == (u128(1) << 126));
	CHECK(u64(-1) == numeric_limits<u64>::max());
}

void test_template() {
	for (string input : {
	         "0\n", "1\n0\n", "1\n3\n-5 0 7\n",
	         "3\n3\n1 2 3\n0\n2\n-8 9\n" }) {
		istringstream in(input);
		auto *old = cin.rdbuf(in.rdbuf());
		cin.clear();
		n = -1;
		CHECK(notebook_main() == 0);
		CHECK(!cin.fail());
		in >> ws;
		CHECK(in.eof());
		if (input == "0\n") CHECK(n == -1);
		else if (input == "1\n0\n") CHECK(n == 0);
		else if (input == "1\n3\n-5 0 7\n") {
			CHECK(n == 3);
			CHECK(a[1] == -5 && a[2] == 0 && a[3] == 7);
		} else {
			CHECK(n == 2 && a[1] == -8 && a[2] == 9);
		}
		cin.rdbuf(old);
		cin.clear();
	}
}

void test_point3d() {
	using P = Point3D<i64>;
	using D = Point3D<double>;
	CHECK(P() == P(0, 0, 0));
	CHECK(P(1, 2, 3) < P(1, 2, 4));
	CHECK(!(P(1, 2, 3) < P(1, 2, 3)));
	CHECK(P(1, 0, 0).cross(P(0, 1, 0)) == P(0, 0, 1));
	double pi = acos(-1.0);
	CHECK(abs(D(0, 1, 0).phi() - pi / 2) < 1e-12);
	CHECK(abs(D(0, 0, -1).theta() - pi) < 1e-12);
	CHECK(D(0, 0, 1).theta() == 0);
	CHECK((D(1, 0, 0).normal(D(0, 2, 0)) - D(0, 0, 1)).dist() < 1e-12);
	for (int it = 0; it < 2000; it ++) {
		P p(ri(-30, 30), ri(-30, 30), ri(-30, 30));
		P q(ri(-30, 30), ri(-30, 30), ri(-30, 30));
		CHECK(p + q == P(p.x + q.x, p.y + q.y, p.z + q.z));
		CHECK((p + q) - q == p);
		CHECK(p * 6 / 3 == p * 2);
		CHECK(p.dot(q) == p.x * q.x + p.y * q.y + p.z * q.z);
		P c = p.cross(q);
		CHECK(c == P(p.y * q.z - p.z * q.y,
		             p.z * q.x - p.x * q.z,
		             p.x * q.y - p.y * q.x));
		CHECK(c.dot(p) == 0 && c.dot(q) == 0);
		CHECK(p.dist2() == p.x * p.x + p.y * p.y + p.z * p.z);
		CHECK(abs(p.dist() * p.dist() - p.dist2()) < 1e-9);
		D v(p.x, p.y, p.z), axis(q.x, q.y, q.z);
		if (p.dist2()) CHECK(abs(v.unit().dist() - 1) < 1e-12);
		double angle = ri(-100, 100) / 10.0;
		D rotated = v.rotate(angle, D(0, 0, 2));
		D want(v.x * cos(angle) - v.y * sin(angle),
		       v.x * sin(angle) + v.y * cos(angle), v.z);
		CHECK((rotated - want).dist() < 1e-9);
		if (q.dist2()) {
			D got = v.rotate(angle, axis);
			CHECK(abs(got.dist() - v.dist()) < 1e-9);
			CHECK(abs(got.dot(axis) - v.dot(axis)) < 1e-9);
			CHECK((got.rotate(-angle, axis) - v).dist() < 1e-9);
		}
	}
}

int main() {
	ios::sync_with_stdio(false), cin.tie(0), cout.tie(0);
	test_base();
	test_template();
	test_point3d();
	done("base, entry point and 3D vectors");
	return 0;
}

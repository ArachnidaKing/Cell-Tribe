#include "sl.h"
#include "slr.h"
#include <cstdio>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <list>
#include <set>
#include <windows.h>
#include <algorithm>

#ifndef _DEBUG
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif


double random(double l, double r) {
	return ((double)rand() / RAND_MAX) * (r - l) + l;
}

using namespace std;
const int WINDOW_WIDTH = 700;
const int WINDOW_HEIGHT = 600;
double moveTowards(double current, double target, double step) {
	double result = current + (target - current) * step;
	if (current < target && result > target) result = target;
	else if (current > target&& result < target) result = target;
	return result;
}
Point moveTowards(const Point& current, const Point& target, double step) {
	return Point(moveTowards(current.x, target.x, step), moveTowards(current.y, target.y, step));
}

class Bezier3 {
	Point s, p1, p2, e;
	static double pow2(double x) { return x * x; }
	static double pow3(double x) { return x * x * x; }
public:
	Bezier3() {}
	Bezier3(const Point& s, const Point& e) :s(e), e(e) {}
	void SetSE(const Point& s, const Point& e) {
		this->s = s;
		this->e = e;
	}
	void rand() {
		double len1 = (::rand() % (int)(((e - s).len() * 0.3) + 1));
		double len2 = (::rand() % (int)(((e - s).len() * 0.3) + 1));
		Point m = (e + s) / 2;
		if (::rand() & 1) {
			p1 = (e - s).rotate(PI / 2 * random(0.6, 1.4));
			p1 = m + p1 / p1.len() * len1;
			p2 = (s - e).rotate(PI / 2 * random(0.6, 1.4));
			p2 = m + p2 / p2.len() * len2;
		} else {
			p1 = (s - e).rotate(PI / 2 * random(0.6, 1.4));
			p1 = m + p1 / p1.len() * len1;
			p2 = (e - s).rotate(PI / 2 * random(0.6, 1.4));
			p2 = m + p2 / p2.len() * len2;
		}
	}
	Point GetPoint(double t) {
		return s * pow3(1 - t) + p1 * 3 * t * pow2(1 - t) + p2 * 3 * pow2(t) * (1 - t) + e * pow3(t);
	}
	Point GetS() {
		return s;
	}
	Point GetE() {
		return e;
	}
};
class Bezier2 {
	Point s, p1, e;
public:
	Bezier2() {}
	Bezier2(const Point& s, const Point& e) :s(e), e(e) {}
	void SetSE(const Point& s, const Point& e) {
		this->s = s;
		this->e = e;
	}
	void rand() {
		double len1 = ::rand() % (int)(((e - s).len()) + 50);
		if (::rand() & 1) {
			p1 = (e - s).rotate(PI / 2 * ((double)::rand() / RAND_MAX));
			p1 = s + p1 / p1.len() * len1;
		} else {
			p1 = (s - e).rotate(PI / 2 * ((double)::rand() / RAND_MAX));
			p1 = e + p1 / p1.len() * len1;
		}
	}
	Point GetPoint(double t) {
		return s * ((1 - t) * (1 - t)) + p1 * (2 * t * (1 - t)) + e * (t * t);
	}
	Point GetS() {
		return s;
	}
	Point GetE() {
		return e;
	}
};
Point Bezier(const Point& p0, const Point& p1, const Point& p2, double t) {
	return p0 * ((1 - t) * (1 - t)) + p1 * (2 * t * (1 - t)) + p2 * (t * t);
}
int rand_next(int seed) {
	seed %= 32768;
	seed = seed * 1103515245 + 12345;
	return((unsigned)(seed / 65536) % 32768);
}
class DynamicPoint :public SLObject {
	SLCircle c;
	Bezier3 bzr;
	double now;
public:
	DynamicPoint() :c(Point(), 0, 0, SLColor(), SLColor()), now(1) {}
	DynamicPoint(const Point& p, double r, int num, const SLColor& col) :c(p, r, num, col, col), now(1) {}
	void SetGoal(const Point& p) {
		bzr.SetSE(c.p, p);
		bzr.rand();
		now = 0;
	}
	void ChangeGoal(const Point& p) {
		bzr.SetSE(bzr.GetS(), p);
	}
	double GetNow() {
		return now;
	}
	Point GetGoal() {
		return bzr.GetE();
	}
	Point GetPos() {
		return c.p;
	}
	void move(double step) {
		now = min(now + step, 1);
		c.p = bzr.GetPoint(now);
	}
	virtual void draw() override {
		return c.draw();
	}
};

//class Particle :public SLObject {
//	vector<SLCircle>vc;
//public:
//	Point p;
//	double r;
//public:
//	Particle() :r(0) {
//	}
//	Particle(const Point& p, double r) {
//	}
//	void update(double t) {
//		for (auto& c : vc) {
//			double x = (c.p - p).len();
//
//		}
//	}
//	virtual void draw() override {
//
//	}
//};


vector<DynamicPoint*>vp;
void update(double t) {
	static bool flag_static = 0;
	static Point pos_static;
	if (slGetMouseButton(SL_MOUSE_BUTTON_LEFT)) {
		Point mp(slGetMouseX(), slGetMouseY());
		bool flag = 1;
		for (auto p : vp) {
			if (p->GetNow() < 0.2) {
				flag = 0;
				break;
			}
		}
		if (flag_static || flag) {
			flag_static = 0;
			for (auto p : vp)  p->SetGoal(mp + Point(rand() % 30 - 15, rand() % 30 - 15));
		} else {
			if (flag_static) {
				for (auto p : vp) {
					//if ((p->GetPos() - pos_static).len() < 61 && p->GetNow() > 0.5)p->SetGoal(p->GetPos() + Point(rand() % 50 - 25, rand() % 50 - 25));
					p->move(0.5 * t);
				}
			} else {
				for (auto p : vp) p->move(0.3 * t);
			}
		}
	} else {
		bool flag = 1;
		for (auto p : vp) {
			if (p->GetNow() != 1) {
				flag = 0;
				break;
			}
		}
		if (!flag) {
			if (flag_static)
				for (auto p : vp) {
					//if ((p->GetPos() - pos_static).len() < 61 && p->GetNow() > 0.5)p->SetGoal(p->GetPos() + Point(rand() % 50 - 25, rand() % 50 - 25));
					p->move(0.5 * t);
				} else
					for (auto p : vp) p->move(0.3 * t);
		} else {
			if (!flag_static) {
				flag_static = 1;
				pos_static = vp[0]->GetPos();
			}
			for (auto& p : vp)
				if ((p->GetPos() - pos_static).len() < 50)
					p->SetGoal(p->GetPos() + Point(rand() % 50 - 25, rand() % 50 - 25));
				else
					p->SetGoal(pos_static);
		}
	}
	//p.move(10 * t);
}
double Hue2RGB(double v1, double v2, double vH) {
	if (vH < 0) vH += 1;
	if (vH > 1) vH -= 1;
	if (6.0 * vH < 1) return v1 + (v2 - v1) * 6.0 * vH;
	if (2.0 * vH < 1) return v2;
	if (3.0 * vH < 2) return v1 + (v2 - v1) * ((2.0 / 3.0) - vH) * 6.0;
	return (v1);
}
SLColor HSL2RGB(double H, double S, double L, double A) {
	double R, G, B;
	double var_1, var_2;
	if (S == 0) {
		R = L * 255.0;
		G = L * 255.0;
		B = L * 255.0;
	} else {
		if (L < 0.5) var_2 = L * (1 + S);
		else         var_2 = (L + S) - (S * L);

		var_1 = 2.0 * L - var_2;

		R = Hue2RGB(var_1, var_2, H + (1.0 / 3.0));
		G = Hue2RGB(var_1, var_2, H);
		B = Hue2RGB(var_1, var_2, H - (1.0 / 3.0));
	}
	return SLColor(R, G, B, A);
}



SLColor Ranbow(double p, double alpha) {
	return HSL2RGB(p, 1, 0.5, alpha);
}

char s[1024];
int main(int args, char* argv[]) {
	GetWindowsDirectoryA(s, 1024);
	puts(s);
	srand(time(0));

	SL::SetUpdateCallBack(update);


	for (int i = 0; i < 1000; i++) {
		vp.push_back(new DynamicPoint(Point(0, 0), 2, 10, Ranbow(i / 1000.0, 0.3)));
	}
	random_shuffle(vp.begin(), vp.end());
	for (auto p : vp)
		p->attach();


	SL::StartThread(WINDOW_WIDTH, WINDOW_HEIGHT);

	SL::Wait();

	return 0;
}



/*

   吊古战场
吊古兴亡感慨多
茫茫漠漠奈愁何
琵琶一曲螳螂墓
惆怅西风万里歌

天下正人无一事
鹧鸪声里白云汀
杜鹃啼鸟归来晚
芳草萋萋满地青

*/
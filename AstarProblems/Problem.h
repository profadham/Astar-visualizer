#pragma once
#include "Astar.h"

class Point {
private:
	int		index;
	double	x;
	double	y;
public:
	Point() {}
	Point(int index, double x, double y) {
		this->index = index;
		this->x = x;
		this->y = y;
	}
	double	getX() {
		return this->x;
	}
	double	getY() {
		return this->y;
	}
	int		getIndex() {
		return this->index;
	}
};


class Barrier {
private:
	Point	point1;
	Point	point2;
public:
	Barrier() {}
	Barrier(Point firstPoint, Point secondPoint) {
		this->point1 = firstPoint;
		this->point2 = secondPoint;
	}
	Point	getPoint1() {
		return this->point1;
	}
	Point	getPoint2() {
		return this->point2;
	}
	double	getGradient() {
		if (this->point1.getX() == this->point2.getX()) {
			return -100000;
		}
		else {
			double gradient = (this->point2.getY() - this->point1.getY()) / (this->point2.getX() - this->point1.getX());
			return gradient;
		}
	}

};


struct Node {
	Point point;
	double g = 0;
	double h = 0;
	double f = 0;
};

class Robot : public Problem<Node> {
private:
	int					noOfNodes;
	int					noOfBarriers;
	vector<Barrier>		barriers;
	vector<Node>		temp;
public:
	Robot(int nodesNo, int BarriersNo) {
		this->noOfNodes = nodesNo;
		this->noOfBarriers = BarriersNo;


		this->nodes.push_back({ Point(0, 20, 10), 0, 0, 0 });
		startNode = this->nodes[0];
		int elapsed_seconds = time(nullptr);
		srand(elapsed_seconds);
		for (int i = 1; i < this->noOfNodes - 1; i++) {
			double x = rand() % ((WIDTH - 50) - 20 + 1) + 20;
			double y = rand() % ((HEIGHT - 50) - 20 + 1) + 20;
			this->nodes.push_back({ Point(i, x, y), 0, 0, 0 });
		}

		this->nodes.push_back({ Point(this->noOfNodes - 1, WIDTH - 30, HEIGHT - 30), 0, 0, 0 });
		this->endNode = this->nodes[this->noOfNodes - 1];




		for (int i = 0; i < this->noOfBarriers; i++) {
			int index1 = rand() % (this->noOfNodes - 1);
			int index2 = rand() % (this->noOfNodes - 1);

			this->barriers.push_back(Barrier(this->nodes[index1].point, this->nodes[index2].point));
		}
	}
	bool				testMode = 1;
	vector<Barrier>		getBarriers() {
		return barriers;
	}
	Barrier				getBarrier(int index) {
		return this->barriers[index];
	}
	double				distance(Point point1, Point point2) {
		double distance = sqrt(pow(point1.getX() - point2.getX(), 2) + pow(point1.getY() - point2.getY(), 2));
		return distance;
	}
	vector<Node>		getNodes() {
		return this->nodes;
	}
	double				getMin(vector<double> list) {
		double min = list[0];
		for (int i = 0; i < list.size(); i++) {
			if (list[i] < min) {
				min = list[i];
			}
		}

		return min;
	}
	double				calculateStepCost(Node currentNode, Node prevNode) {
		double cost = this->distance(currentNode.point, prevNode.point);
		return cost;
	}
	double				calculateHeuristic(Node currentNode) {
		double h = this->distance(currentNode.point, this->endNode.point);

		/*bool HitBarrier = false;
		int index = 0;
		Barrier firstBarrier;
		vector<Point> endPoints;
		vector<double> distances;

		for (int i = 0; i < barriers.size(); i++) {
			if (hitBarrier(currentNode.point, endNode.point, barriers[index])) {
				HitBarrier = true;
				firstBarrier = barriers[index];

				endPoints.push_back(barriers[index].getPoint1());
				endPoints.push_back(barriers[index].getPoint2());
			}
		}

		if (HitBarrier) {

			for (int i = 0; i < endPoints.size(); i++) {
				distances.push_back(distance(currentNode.point, endPoints[i]) + distance(endPoints[i], endNode.point));
			}

			h = getMin(distances);

		}*/


		return h;

	}
	bool				hitBarrier(Point point1, Point point2, Barrier barrier) {

		if (point2.getIndex() == barrier.getPoint1().getIndex() || point2.getIndex() == barrier.getPoint2().getIndex()) return false;
		if (point1.getIndex() == barrier.getPoint1().getIndex() || point1.getIndex() == barrier.getPoint2().getIndex()) return false;

		double mLine = 0.0;
		double mBarrier = 0.0;
		double cLine = 0.0;
		double cBarrier = 0.0;
		double intersectX;
		double intersectY;
		vector<double> barrierData; // x1, x2, y1, y2
		vector<double> lineData; //x1, x2, y1, y2
		bool inTheLine;
		mBarrier = barrier.getGradient();
		if (point1.getX() != point2.getX()) {
			mLine = (point2.getY() - point1.getY()) / (point2.getX() - point1.getX());
		}
		else {
			mLine = -100000;
		}

		if (mLine != -100000) {
			cLine = point1.getY() - (mLine * point1.getX());
		}
		else {
			cLine = -100000;
		}

		if (mBarrier != -100000) {
			cBarrier = barrier.getPoint1().getY() - (mBarrier * barrier.getPoint1().getX());
		}
		else {
			cBarrier = -100000;
		}

		if (mBarrier == mLine) {
			return false;
		}
		else if (mBarrier != -100000 && mLine == -100000) {
			intersectY = (mBarrier * point1.getX()) + cBarrier;
			intersectX = point1.getX();
		}
		else if (mBarrier == -100000 && mLine != -100000) {
			intersectY = (mLine * barrier.getPoint1().getX()) + cLine;
			intersectX = barrier.getPoint1().getX();
		}
		else {
			intersectX = (cLine - cBarrier) / (mBarrier - mLine);
			intersectY = (mBarrier * intersectX) + cBarrier;
		}


		lineData.push_back(point1.getX());
		lineData.push_back(point2.getX());
		lineData.push_back(point1.getY());
		lineData.push_back(point2.getY());

		barrierData.push_back(barrier.getPoint1().getX());
		barrierData.push_back(barrier.getPoint2().getX());
		barrierData.push_back(barrier.getPoint1().getY());
		barrierData.push_back(barrier.getPoint2().getY());

		inTheLine = false;

		if (lineData[0] <= lineData[1]) {
			if (lineData[2] <= lineData[3]) {
				if (lineData[0] < intersectX && lineData[1] > intersectX && lineData[2] < intersectY && lineData[3] > intersectY) {
					inTheLine = true;
				}
			}
			else {
				if (lineData[0] < intersectX && lineData[1] > intersectX && lineData[2] > intersectY && lineData[3] < intersectY) {
					inTheLine = true;
				}
			}
		}
		else {
			if (lineData[2] <= lineData[3]) {
				if (lineData[0] > intersectX && lineData[1] < intersectX && lineData[2] < intersectY && lineData[3] > intersectY) {
					inTheLine = true;
				}
			}
			else {
				if (lineData[0] > intersectX && lineData[1] < intersectX && lineData[2] > intersectY && lineData[3] < intersectY) {
					inTheLine = true;
				}
			}
		}


		if (inTheLine) {
			if (barrierData[0] <= barrierData[1]) {
				if (barrierData[2] <= barrierData[3]) {
					if (barrierData[0] < intersectX && barrierData[1] > intersectX && barrierData[2] < intersectY && barrierData[3] > intersectY) {

						return true;
					}
				}
				else {
					if (barrierData[0] < intersectX && barrierData[1] > intersectX && barrierData[2] > intersectY && barrierData[3] < intersectY) {

						return true;
					}
				}
			}
			else {
				if (barrierData[2] <= barrierData[3]) {
					if (barrierData[0] > intersectX && barrierData[1] < intersectX && barrierData[2] < intersectY && barrierData[3] > intersectY) {

						return true;
					}
				}
				else {
					if (barrierData[0] > intersectX && barrierData[1] < intersectX && barrierData[2] > intersectY && barrierData[3] < intersectY) {

						return true;
					}
				}
			}
		}

		return false;

	}
	bool				connectionBetween(Node node1, Node node2)
	{
		bool HitBarrier = false;
		for (int i = 0; i < barriers.size(); i++)
		{
			if (hitBarrier(node1.point, node2.point, barriers[i]))
			{
				HitBarrier = true;
			}
		}

		return !HitBarrier;
	}
	vector<Node>		getChildren(Node currentNode) {
		vector<Node> children;

		for (int i = 1; i < nodes.size(); i++) {
			if (currentNode.point.getIndex() != i) {

				bool meatsWithBarrier = false;

				int index = 0;
				while (!meatsWithBarrier && index < this->barriers.size()) {
					if (this->hitBarrier(currentNode.point, this->nodes[i].point, this->barriers[index])) {
						meatsWithBarrier = true;

					}
					else {
						index++;
					}
				}

				if (!meatsWithBarrier) {
					children.push_back(this->nodes[i]);
				}
			}
		}
		return children;
	}




};

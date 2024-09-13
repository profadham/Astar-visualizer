#pragma once
#include <iostream>
#include <vector>
using namespace std;
const int				WIDTH = 1400;
const int				HEIGHT = 900;


template<typename T>
vector<T>				removeElement(vector<T> elements, int index) {
	vector<T> temp;
	for (int i = 0; i < elements.size(); i++) {
		if (i != index) {
			temp.push_back(elements[i]);
		}
	}
	return temp;
}

template<typename T>
vector<T>				removeElement(vector<T> elements, T element) {
	vector<T> temp;
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i].point.getIndex() != element.point.getIndex()) {
			temp.push_back(elements[i]);
		}
	}
	return temp;
}

template<typename N>
class Problem {
public:
	bool				solved;
	N					startNode;
	N					endNode;
	vector<N>			nodes;
	vector<N>			Solution;

	vector<N>			getSolution() {
		return Solution;
	}
	N					getStartNode() {
		return this->startNode;
	}
	N					getEndNode() {
		return this->endNode;
	}
	vector<N>			getNodes() {
		return nodes;
	}
	void				setSolution(vector<N> solution) {
		solved = true;
		Solution.clear();
		for (int i = 0; i < solution.size(); i++) {
			Solution.push_back(solution[i]);
		}
	}

	virtual double		calculateHeuristic(N currentNode) {
		return 0;
	}
	virtual double		calculateStepCost(N currentNode, N prevNode) {
		return 0;
	}
	virtual vector<N>	getChildren(N currentNode) {
		vector<N> fady;
		return fady;
	}
	virtual bool		connectionBetween(N node1, N node2) { return 0; }
};

/*
class Point {
private:
	int index;
	double x;
	double y;
public:

	double getX() {
		return this->x;
	}
	double getY() {
		return this->y;
	}
	int getIndex() {
		return this->index;
	}

	Point() {}

	Point(int index, double x, double y) {
		this->index = index;
		this->x = x;
		this->y = y;
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////

class Barrier {
private:
	Point point1;
	Point point2;
public:
	Barrier() {}
	Barrier(Point firstPoint, Point secondPoint) {
		this->point1 = firstPoint;
		this->point2 = secondPoint;
	}

	Point getPoint1() {
		return this->point1;
	}
	Point getPoint2() {
		return this->point2;
	}
	double getGradient() {
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
	int noOfNodes;
	int noOfBarriers;
	vector<Barrier> barriers;
	vector<Node> temp;
public:
	bool testMode = 1;
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

	vector<Barrier> getBarriers() {
		return barriers;
	}
	Barrier getBarrier(int index) {
		return this->barriers[index];
	}
	double distance(Point point1, Point point2) {
		double distance = sqrt(pow(point1.getX() - point2.getX(), 2) + pow(point1.getY() - point2.getY(), 2));
		return distance;
	}
	vector<Node> getNodes() {
		return this->nodes;
	}

	double getMin(vector<double> list) {
		double min = list[0];
		for (int i = 0; i < list.size(); i++) {
			if (list[i] < min) {
				min = list[i];
			}
		}

		return min;
	}

	double calculateStepCost(Node currentNode, Node prevNode) {
		double cost = this->distance(currentNode.point, prevNode.point);
		return cost;
	}
	double calculateHeuristic(Node currentNode) {
		double h = this->distance(currentNode.point, this->endNode.point);

		bool HitBarrier = false;
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

		}


		return h;

	}

	bool hitBarrier(Point point1, Point point2, Barrier barrier) {
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

		if (lineData[0] < lineData[1]) {
			if (lineData[2] < lineData[3]) {
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
			if (lineData[2] < lineData[3]) {
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
			if (barrierData[0] < barrierData[1]) {
				if (barrierData[2] < barrierData[3]) {
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
				if (barrierData[2] < barrierData[3]) {
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




	vector<Node> getChildren(Node currentNode) {
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




};*/


template <typename N>
struct Connection
{
	N				firstNode;
	N				secondNode;
};

template <typename N>
class Astar {

private:
	vector<N>				openList;
	vector<N>				closeList;
	vector<Connection<N>>	connections;
public:
	bool					testMode = 1;
	Astar() {}
	int						getLeastF(vector<N> nodesAvailable) {

		double min = nodesAvailable[0].f;
		int minIndex = 0;

		for (int i = 1; i < nodesAvailable.size(); i++) {
			if (nodesAvailable[i].f < min) {
				min = nodesAvailable[i].f;
				minIndex = i;
			}
		}

		return minIndex;
	}
	vector<N>				getClose() {
		return closeList;
	}
	bool					foundInList(vector<N> List, N val) {
		bool found = false;
		int index = 0;

		while (index < List.size() && found == false) {
			if (List[index].point.getIndex() == val.point.getIndex()) {  ////here
				found = true;
			}
			else {
				index++;
			}
		}
		return found;
	}
	int						search(vector<N> List, N val) {
		int returnIndex = -1;
		int index = 0;

		while (index < List.size() && returnIndex == -1) {
			if (List[index].point.getIndex() == val.point.getIndex()) {   ////here
				returnIndex = index;
			}
			else {
				index++;
			}
		}
		return returnIndex;
	}
	bool					isConnection(N node1, N node2)
	{
		bool found = false;
		int index = 0;
		while (index < connections.size() && found == false)
		{
			if ((connections[index].firstNode.point.getIndex() == node1.point.getIndex() && connections[index].secondNode.point.getIndex() == node2.point.getIndex()) ||
				(connections[index].firstNode.point.getIndex() == node2.point.getIndex() && connections[index].secondNode.point.getIndex() == node1.point.getIndex()))
			{
				found = true;
			}
			else
			{
				index++;
			}
		}

		return found;
	}
	void					reconstructPath(Problem<N>* problem)
	{
		/*vector<N> temp;
		N currentNode = closeList.back();
		
		while (currentNode.point.getIndex() != closeList.front().point.getIndex())
		{
			temp.push_back(currentNode);
			for (int i = 0; i < closeList.size(); i++)
			{
				if (currentNode.g - closeList[i].g == problem->calculateStepCost(currentNode, closeList[i]))
				{
					cout << "sa7";
					currentNode = closeList[i];
				}
			}
		}
		closeList.clear();
		for (int i = temp.size() - 1; i >= 0; i--)
		{
			closeList.push_back(temp[i]);
		}


		if (testMode)
		{
			cout << endl << endl;
			for (int i = 0; i < closeList.size(); i++)
			{
				cout << closeList[i].point.getIndex() << "\t";
			}
		}*/
		vector<N> temp;
		N current;
		current = closeList[closeList.size() - 1];
		temp.push_back(current);

		while (current.point.getIndex() != closeList.front().point.getIndex())
		{
			for (int i = closeList.size() - 1; i >= 0; i--)
			{
				if (isConnection(closeList[i], current))
				{
					temp.push_back(closeList[i]);
					N Temp = current;
					current = closeList[i];
					closeList = removeElement(closeList, Temp);
					
				}
			}
		}

		closeList.clear();
		for (int x = temp.size() - 1; x >= 0; x--)
		{
			closeList.push_back(temp[x]);
		}

		//closeList = temp;
		
	}
	vector<N>				Solve(Problem<N>* problem, N goal) {


		openList.push_back(problem->getStartNode());

		N currentNode;
		vector<N> adjacent;
		int currentIndex;
		while (openList.size() != 0) {

			currentIndex = getLeastF(openList);
			currentNode = openList[currentIndex];

			openList = removeElement(openList, currentIndex);
			//openList.clear();
			closeList.push_back(currentNode);


			if (currentNode.point.getIndex() == goal.point.getIndex()) {  ////here


				reconstructPath(problem);
				problem->setSolution(closeList);

				return closeList;

			}

			adjacent = problem->getChildren(currentNode);


			for (int x = 0; x < adjacent.size(); x++) {
				if (foundInList(closeList, adjacent[x])) {
					continue;
				}

				adjacent[x].g = currentNode.g + problem->calculateStepCost(adjacent[x], currentNode);
				adjacent[x].h = problem->calculateHeuristic(adjacent[x]);
				adjacent[x].f = adjacent[x].g + adjacent[x].h;

				if (foundInList(openList, adjacent[x])) {
					N adjacentInOpenList = openList[search(openList, adjacent[x])];

					if (adjacent[x].g > adjacentInOpenList.g) {
						continue;
					}
				}
				openList.push_back(adjacent[x]);
				connections.push_back({ currentNode, adjacent[x] });
			}



		}


	}

};

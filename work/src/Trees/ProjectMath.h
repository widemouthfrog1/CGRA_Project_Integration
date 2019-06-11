#pragma once
#include "Circle.h"

using namespace std;
using namespace glm;

class Circle;

namespace Math {
	struct Spline {
		std::vector<glm::vec3> controlPoints;
		std::vector<glm::vec3> interpolatedPoints;
	};
	float getT(float t, vec3 point1, vec3 point2)
	{
		float a = pow((point2.x - point1.x), 2.0f) + pow((point2.y - point1.y), 2.0f) + pow((point2.z - point1.z), 2.0f);
		float b = pow(a, 0.5f);
		float c = pow(b, 0.5);

		return (c + t);
	}
	Spline spline(const std::vector<glm::vec3> points, int divisions) {
		vector<vec3> controlPoints;
		vector<vec3> interpolatedPoints;
		vec3 p0 = points.at(0);
		vec3 p1 = points.at(1);
		vec3 p2 = points.at(2);
		vec3 p3 = points.at(3);

		float t0 = 0;
		float t1 = getT(t0, p0, p1);
		float t2 = getT(t1, p1, p2);
		float t3 = getT(t2, p2, p3);
		int ind = 0;
		for (float t = t1; t < t2; t += ((t2 - t1) / divisions))
		{
			vec3 a1 = (t1 - t) / (t1 - t0)*p0 + (t - t0) / (t1 - t0)*p1;
			vec3 a2 = (t2 - t) / (t2 - t1)*p1 + (t - t1) / (t2 - t1)*p2;
			vec3 a3 = (t3 - t) / (t3 - t2)*p2 + (t - t2) / (t3 - t2)*p3;

			vec3 b1 = (t2 - t) / (t2 - t0)*a1 + (t - t0) / (t2 - t0)*a2;
			vec3 b2 = (t3 - t) / (t3 - t1)*a2 + (t - t1) / (t3 - t1)*a3;

			vec3 c = (t2 - t) / (t2 - t1)*b1 + (t - t1) / (t2 - t1)*b2;
			interpolatedPoints.push_back(c);
		}
		for (int i = 0; i < points.size(); i++) {
			controlPoints.push_back(points.at(i));
		}


		Spline spline;
		spline.controlPoints = controlPoints;
		spline.interpolatedPoints = interpolatedPoints;
		return spline;
	}

	std::vector<float> angleOfBaseAndBranchConnectors(Circle circle0, Circle circle1) {
		std::vector<float> angles;
		vec3 circle0to1 = circle1.center - circle0.center;
		vec3 normal0 = cross(circle0.getPoint(0.0f).pos - circle0.center, circle0.getPoint(1.0f).pos - circle0.center);
		vec3 projection0 = projection(circle0to1, normal0);
		vec3 planeProjection0 = normalize(circle0to1 - projection0)*circle0.radius;
		angles.push_back(circle0.getAngle(translate(mat4(1), circle0.center)*vec4(planeProjection0, 1)));

		vec3 normal1 = cross(circle1.getPoint(0.0f).pos - circle1.center, circle1.getPoint(1.0f).pos - circle1.center);
		vec3 centerToNormal0 = normal0 - circle1.center;
		vec3 projectionOnNormal1 = projection(centerToNormal0, normal1);
		vec3 planeProjection1 = normalize(centerToNormal0 - projectionOnNormal1)*circle1.radius;
		angles.push_back(circle1.getAngle(translate(mat4(1), circle1.center)*vec4(planeProjection1, 1)));
		angles.push_back(circle1.getAngle(translate(mat4(1), circle1.center)*vec4(planeProjection1, 1)) + pi<float>());
		return angles;
	}


	std::vector<glm::vec3> circle(int numberOfPoints, int radius) {
		vector<vec3> circle;
		float step = 2 * pi<float>() / numberOfPoints;
		for (int i = 0; i < 2 * pi<float>(); i += step) {
			circle.push_back(vec3(sin(i), 0, cos(i)));
		}
		return circle;
	}
	/*Vertex closestToBasePlane(std::vector<Vertex> base, std::vector<Vertex> branch);
	Vertex closestBasePoint(std::vector<Vertex> base, Vertex closest);
	int ninetyDegreeRotation(std::vector<Vertex> circle, int point, glm::vec3 direction);
	int getNeighbour(int circleSize, int point, bool leftNeighbour);*/
	vec3 projection(vec3 a, vec3 b) {
		return (
			dot(a, b)
			/ (length(b)*length(b)
				)
			)*b;
	}
	
}
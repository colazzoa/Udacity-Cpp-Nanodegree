#include "route_planner.h"
#include <algorithm>

using std::cout;
using std::reverse;
using std::sort;

bool Compare(RouteModel::Node *pn1, RouteModel::Node *pn2)
{
    float f1 = pn1->g_value + pn1->h_value;
    float f2 = pn2->g_value + pn2->h_value;
    return f1 > f2;
}

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y) : m_Model(model)
{
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    start_node = &m_Model.FindClosestNode(start_x, start_y);
    end_node = &m_Model.FindClosestNode(end_x, end_y);
}

float RoutePlanner::CalculateHValue(RouteModel::Node const *node)
{
    return node->distance((*end_node));
}

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node)
{
    current_node->FindNeighbors();
    auto current_g = current_node->g_value;
    current_node->visited = true;

    for (RouteModel::Node *node : current_node->neighbors)
    {
        node->g_value = current_g + node->distance(*current_node);
        node->h_value = this->CalculateHValue(node);
        node->parent = current_node;
        node->visited = true;
        open_list.push_back(node);
    }
}

RouteModel::Node *RoutePlanner::NextNode()
{
    RouteModel::Node *next_node;
    sort(this->open_list.begin(), this->open_list.end(), Compare);
    next_node = open_list.back();
    return next_node;
}

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node)
{
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    while (current_node->x != this->start_node->x && current_node->y != this->start_node->y)
    {
        path_found.push_back(*current_node);
        distance += current_node->distance(*current_node->parent);
        current_node = current_node->parent;
    }

    path_found.push_back(*current_node);
    reverse(path_found.begin(), path_found.end());

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;
}

void RoutePlanner::AStarSearch()
{
    RouteModel::Node *current_node = nullptr;
    open_list.push_back(this->start_node);
    auto end_x = end_node->x;
    auto end_y = end_node->y;

    while (open_list.size() > 0)
    {
        current_node = NextNode();
        open_list.pop_back();

        if (current_node->x == end_x && current_node->y == end_y)
        {
            m_Model.path = ConstructFinalPath(current_node);
            break;
        }

        AddNeighbors(current_node);
    }
}
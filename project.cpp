#include "project.hh"
#include <iostream>

Project::Project(const std::string& id, const std::string& start):
    id_(id), start_(start)
{
}

Project::Project(const std::string& id, const Date &start):
    id_(id), start_(start)
{
}

Project::~Project()
{
    //std::cout << "Project " << id_ << " destructed." << std::endl;
}

std::set<std::string> Project::get_requirements() const {
    return requirements_;  // Assuming 'requirements_' is a member variable holding project requirements
}

std::string Project::get_sorted_requirements() const {
    std::vector<std::string> sorted_requirements(requirements_.begin(), requirements_.end());
    std::sort(sorted_requirements.begin(), sorted_requirements.end());

    std::string result;
    for (const auto& requirement : sorted_requirements) {
        if (!result.empty()) {
            result += ", ";
        }
        result += requirement;
    }
    return result.empty() ? "None" : result;
}

std::string Project::get_sorted_employees() const {
    std::vector<std::string> sorted_employees(assigned_employees_.begin(), assigned_employees_.end());
    std::sort(sorted_employees.begin(), sorted_employees.end());

    std::string result;
    for (const auto& employee : sorted_employees) {
        if (!result.empty()) {
            result += ", ";
        }
        result += employee;
    }
    return result.empty() ? "None" : result;
}

const std::set<std::string>& Project::get_assigned_employees() const {
    return assigned_employees_;
}

void Project::remove_employee(const std::string& employee_id) {
    assigned_employees_.erase(employee_id);
}

void Project::add_requirement(const std::string& requirement, std::map<std::string, Employee*>& all_employees) {
    auto result = requirements_.insert(requirement);
    if (result.second) {
        // Requirement was successfully added, now remove unqualified employees
        std::vector<std::string> unqualified_employees;
        for (const auto& employee_id : assigned_employees_) {
            Employee* employee = all_employees[employee_id];
            if (!employee->meets_requirements(requirements_)) {
                unqualified_employees.push_back(employee_id);
            }
        }

        for (const auto& employee_id : unqualified_employees) {
            remove_employee(employee_id);
            // You can also print a message here if needed
            std::cout << NOT_QUALIFIED << employee_id << std::endl;
        }
    }
}

#ifndef PROJECT_HH
#define PROJECT_HH

#include "employee.hh"
#include "date.hh"
#include "utils.hh"
#include <string>
#include <algorithm>
#include <map>

const std::string NOT_QUALIFIED = "Not qualified any more: ";

class Project
{
public:
    /**
     * @brief Project constructor
     * @param : id
     * @param : start (given as string ddmmyyyy)
     */
    Project(const std::string& id, const std::string& start);

    /**
     * @brief Project constructor
     * @param : id
     * @param : start (given as Date object)
     */
    Project(const std::string& id, const Date& start);

    /**
     * @brief Project destructor
     */
    ~Project();

    // Method to set the end date of the project
    void set_end_date(const Date& end_date) {
        end_ = end_date;
    }

    // Method to check if the project is closed
    bool is_closed() const {
        return !end_.is_default();
    }

    // Method to get the project ID
    const std::string& get_id() const {
        return id_;
    }

    // Method to get the start date
    const Date& get_start_date() const {
        return start_;
    }

    // Method to get the end date
    const Date& get_end_date() const {
        return end_;
    }

    // Method to get the project's requirements
    std::set<std::string> get_requirements() const;

    // Method to check if the project has any requirements
    bool has_requirements() const {
        return !requirements_.empty();
    }

    // Method to add a requirement to a project and remove unqualified employees
    void add_requirement(const std::string& requirement, std::map<std::string, Employee*>& all_employees);

    // Method to check if a project has a specific requirement
    bool has_requirement(const std::string& requirement) const {
        return requirements_.find(requirement) != requirements_.end();
    }

    // Method to assign an employee to the project
    bool assign_employee(const std::string& employee_id) {
        // Attempt to insert returns a pair, where the second is a bool that is true if insertion took place
        auto result = assigned_employees_.insert(employee_id);
        return result.second; // True if the employee was successfully assigned
    }

    // Method to check if an employee is already assigned to this project
    bool is_employee_assigned(const std::string& employee_id) const {
        return assigned_employees_.find(employee_id) != assigned_employees_.end();
    }

    // Method to remove an employee from the project
    void remove_employee(const std::string& employee_id);

    // New methods for PRINT_PROJECT_INFO implementation
    std::string get_sorted_requirements() const;
    std::string get_sorted_employees() const;
    const std::set<std::string>& get_assigned_employees() const;

private:
    /**
     * @brief obvious attributes
     */
    std::string id_; // Can be a name or any other identifier
    Date start_;
    Date end_;

    // More attributes and private methods

    // A set to hold the requirements
    std::set<std::string> requirements_;

    // A set to hold the IDs of assigned employees
    std::set<std::string> assigned_employees_;
};

#endif // PROJECT_HH

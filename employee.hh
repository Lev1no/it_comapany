#ifndef EMPLOYEE_HH
#define EMPLOYEE_HH

#include "date.hh"
#include <string>
#include <set>

class Employee
{
public:
    /**
     * @brief Employee default constructor
     */
    Employee();

    /**
     * @brief Employee constructor
     * @param id
     */
    Employee(const std::string& id);

    /**
     * @brief Employee constructor (not in use)
     * @param id
     * @param date_of_birth
     */
    Employee(const std::string& id, const std::string& date_of_birth);

    /**
     * @brief Employee destructor
     */
    ~Employee();

    /**
     * @brief get_id
     * @return employee's id
     */
    std::string get_id() const;

    /**
     * @brief add_skill
     * @param skill (name)
     * Adds a skill for an employee
     */
    void add_skill(const std::string& skill);

    /**
     * @brief has_skill
     * @param : skill (name)
     * @return true if the employee has the skill, otherwise returns false
     */
    bool has_skill(const std::string& skill) const;

    /**
     * @brief print_id
     * @param pre_text
     * Prints employee's id, before that prints the pre_text
     */
    void print_id(const std::string& pre_text) const;

    /**
     * @brief print_skills
     * Prints employee's skills, all skills in one line, separated by a comma,
     * if the employee has no skills, prints "None"
     */
    void print_skills() const;

    /**
     * @brief operator<
     * @param : rhs
     * Comparison based on id_'s, enables forming a set of Employee objects
     */
    bool operator<(const Employee& rhs) const;

    // Method to check if the employee meets any of the project's requirements
    bool meets_requirements(const std::set<std::string>& required_skills) const;

    const std::set<std::string>& get_skills() const;

    // Methods for active status management
    void activate();
    void deactivate();
    bool is_active() const;

private:
    /**
     * @brief obvious attributes
     */
    std::string id_;               // Can be a name or any other identifier
    Date date_of_birth_;           // Not actually needed
    std::set<std::string> skills_; // IT skills

    bool is_active_; // Tracks whether the employee is currently active
};

#endif // EMPLOYEE_HH

#include "company.hh"
#include "utils.hh"
#include <iostream>
#include <set>

Company::Company()
{
}

Company::~Company()
{
    for (auto& employee : all_staff_) {
        delete employee.second;  // Deallocate each employee
    }
    all_staff_.clear();

    for (auto& project : projects_) {
        delete project.second;  // Deallocate each project
    }
    projects_.clear();

}

void Company::set_date(Params params)
{
    std::string day = params.at(0);
    std::string month = params.at(1);
    std::string year = params.at(2);
    if( not Utils::is_numeric(day, false) or
        not Utils::is_numeric(month, false) or
        not Utils::is_numeric(year, false) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    Utils::today.set(stoi(day), stoi(month), stoi(year));
    std::cout << "Date has been set to ";
    Utils::today.print();
    std::cout << std::endl;
}

void Company::advance_date(Params params)
{
    std::string amount = params.at(0);
    if( not Utils::is_numeric(amount, true) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    Utils::today.advance(stoi(amount));
    std::cout << "New date is ";
    Utils::today.print();
    std::cout << std::endl;
}

void Company::recruit(Params params)
{
    std::string employee_id = params.at(0);
    auto iter_current = current_staff_.find(employee_id);

    // Check if the employee is already in current staff
    if (iter_current != current_staff_.end()) {
        std::cout << ALREADY_EXISTS << employee_id << std::endl;
        return;
    }

    // Check if the employee already exists in all staff (rehire situation)
    auto iter_all = all_staff_.find(employee_id);
    if (iter_all != all_staff_.end()) {
        // If the employee exists, reactivate and reinsert into current staff
        iter_all->second->activate(); // Reactivate the employee
        current_staff_.insert(*iter_all);
        std::cout << EMPLOYEE_RECRUITED << std::endl;
    } else {
        // If the employee is new, create a new instance and insert into both maps
        Employee* new_employee = new Employee(employee_id);
        all_staff_.insert({employee_id, new_employee});
        current_staff_.insert({employee_id, new_employee});
        std::cout << EMPLOYEE_RECRUITED << std::endl;
    }
}

void Company::leave(Params params)
{
    std::string employee_id = params.at(0);

    // Find the employee in the all_staff_ map to check if they can be deactivated
    auto it_all_staff = all_staff_.find(employee_id);
    if (it_all_staff == all_staff_.end()) {
        std::cout << CANT_FIND << employee_id << std::endl;
        return;
    }

    // Check if the employee is already deactivated
    if (!it_all_staff->second->is_active()) {
        std::cout << CANT_FIND << employee_id << std::endl;
        return;
    }

    // Deactivate the employee
    it_all_staff->second->deactivate();

    // Remove the employee from the current_staff_ map
    auto it_current_staff = current_staff_.find(employee_id);
    if (it_current_staff != current_staff_.end()) {
        current_staff_.erase(it_current_staff);
    }

    std::cout << EMPLOYEE_LEFT << std::endl;
}

void Company::add_skill(Params params)
{
    std::string employee_id = params.at(0);
    std::string skill_name = params.at(1);

    std::map<std::string, Employee*>::const_iterator
            staff_iter = current_staff_.find(employee_id);
    if( staff_iter == current_staff_.end() )
    {
        std::cout << CANT_FIND << employee_id << std::endl;
        return;
    }

    staff_iter->second->add_skill(skill_name);
    std::cout << SKILL_ADDED << employee_id << std::endl;
}

void Company::print_current_staff(Params)
{
    if( current_staff_.empty() )
    {
        std::cout << "None" << std::endl;
        return;
    }

    for( auto employee : current_staff_ )
    {
        employee.second->print_id("");
        std::cout << std::endl;
    }
}

void Company::create_project(Params params)
{
    std::string project_id = params.at(0);

    // Check if the project already exists
    if (projects_.find(project_id) != projects_.end()) {
        std::cout << ALREADY_EXISTS << project_id << std::endl;
        return;
    }

    // Create a new project and add it to the map
    Project* new_project = new Project(project_id, Utils::today);
    projects_.insert({project_id, new_project});
    std::cout << PROJECT_CREATED << std::endl;
}

void Company::close_project(Params params)
{
    std::string project_id = params.at(0);

    // Search for the project
    auto it = projects_.find(project_id);
    if (it == projects_.end()) {
        std::cout << CANT_FIND << project_id << std::endl;
        return;
    }
    // Get the project pointer
    Project* project = it->second;

    // Set the end date of the project if it's not already set
    if (!project->is_closed()) {
        project->set_end_date(Utils::today);
    }

    std::cout << PROJECT_CLOSED << std::endl;
}

void Company::print_projects(Params)
{
    // Check if there are no projects
    if (projects_.empty()) {
        std::cout << "None" << std::endl;
        return;
    }

    // Create a temporary vector to hold the projects for sorting
    std::vector<std::pair<std::string, Project*>> projects_vector(projects_.begin(), projects_.end());

    // Sort the vector based on the start date of the projects
    std::sort(projects_vector.begin(), projects_vector.end(),
              [](const std::pair<std::string, Project*>& a, const std::pair<std::string, Project*>& b) {
                  return a.second->get_start_date() < b.second->get_start_date();
              });

    // Iterate through the sorted projects and print details
    for (const auto& project_entry : projects_vector) {
        const auto& project = project_entry.second;
        std::cout << project->get_id() << " : ";
        project->get_start_date().print();
        std::cout << " - ";
        if (project->is_closed()) {
            project->get_end_date().print();
        }
        std::cout << std::endl;
    }
}

void Company::add_requirement(Params params)
{
    std::string project_id = params.at(0);
    std::string requirement = params.at(1);

    auto project_it = projects_.find(project_id);
    if (project_it == projects_.end()) {
        std::cout << CANT_FIND << project_id << std::endl;
        return;
    }

    Project* project = project_it->second;
    if (project->is_closed()) {
        std::cout << CANT_FIND << project_id << std::endl;
        return;
    }

    if (!project->has_requirement(requirement)) {
        // Add the requirement to the project
        project->add_requirement(requirement, all_staff_);

        // Iterate over the employees assigned to the project and remove those who do not meet the new requirements
        const auto& assigned_employees = project->get_assigned_employees();
        for (const std::string& employee_id : assigned_employees) {
            Employee* employee = all_staff_.find(employee_id)->second;
            if (!employee->meets_requirements(project->get_requirements())) {
                // Remove employee from the project
                project->remove_employee(employee_id);

                // Print that the employee is not qualified anymore
                std::cout << NOT_QUALIFIED << employee_id << std::endl;
            }
        }

        std::cout << REQUIREMENT_ADDED << project_id << std::endl;
    }
    // If the requirement is already present, do nothing, and no message is printed.
}

void Company::assign(Params params)
{
    std::string employee_id = params.at(0);
    std::string project_id = params.at(1);

    // Find the employee in the all_staff_ map
    auto employee_it = all_staff_.find(employee_id);
    if (employee_it == all_staff_.end()) {
        std::cout << CANT_FIND << employee_id << std::endl;
        return;
    }

    // Find the project in the projects_ map
    auto project_it = projects_.find(project_id);
    if (project_it == projects_.end()) {
        std::cout << CANT_FIND << project_id << std::endl;
        return;
    }

    // Pointers to the employee and project
    Project* project = project_it->second;
    Employee* employee = employee_it->second;

    // Check if the project is closed
    if (project->is_closed()) {
        std::cout << CANT_ASSIGN << employee_id << std::endl;
        return;
    }

    // Check if the employee is already assigned to the project
    if (project->is_employee_assigned(employee_id)) {
        std::cout << CANT_ASSIGN << employee_id << std::endl;
        return;
    }

    // Check if the employee meets the project's requirements
    if (project->has_requirements() && !employee->meets_requirements(project->get_requirements())) {
        std::cout << CANT_ASSIGN << employee_id << std::endl;
        return;
    }

    // Assign the employee to the project
    project->assign_employee(employee_id);
    std::cout << STAFF_ASSIGNED << project_id << std::endl;
}

void Company::print_project_info(Params params)
{
    std::string project_id = params.at(0);
    auto it = projects_.find(project_id);
    if (it == projects_.end()) {
        std::cout << CANT_FIND << project_id << std::endl;
        return;
    }

    Project* project = it->second;
    std::cout << project->get_id() << " : ";
    project->get_start_date().print();
    std::cout << " - ";
    if (project->is_closed()) {
        project->get_end_date().print();
    }
    std::cout << std::endl;

    std::cout << "** Requirements: " << project->get_sorted_requirements() << std::endl;
    std::cout << "** Staff: " << project->get_sorted_employees() << std::endl;
}

void Company::print_employee_info(Params params)
{
    std::string employee_id = params.at(0);
    auto it = all_staff_.find(employee_id);

    // Check if the employee exists in the all_staff_ map
    if (it == all_staff_.end()) {
        std::cout << CANT_FIND << employee_id << std::endl;
        return;
    }

    Employee* employee = it->second;

    // If the employee is not active, don't print their info
    if (!employee->is_active()) {
        std::cout << CANT_FIND << employee_id << std::endl;
        return;
    }

    // Print the employee's skills
    std::cout << "Skills: ";
    if (employee->get_skills().empty()) {
        std::cout << "None";
    } else {
        // Sort and print skills
        std::vector<std::string> sorted_skills(employee->get_skills().begin(), employee->get_skills().end());
        std::sort(sorted_skills.begin(), sorted_skills.end());
        for (auto skill = sorted_skills.begin(); skill != sorted_skills.end(); ++skill) {
            std::cout << (skill != sorted_skills.begin() ? ", " : "") << *skill;
        }
    }
    std::cout << std::endl;

    // Print the projects that the employee has worked on
    std::cout << "Projects:";
    bool has_projects = false;
    for (const auto& project_pair : projects_) {
        Project* project = project_pair.second;
        if (project->is_employee_assigned(employee_id)) {
            has_projects = true;
            std::cout << std::endl << "** " << project->get_id() << " : ";
            project->get_start_date().print();
            std::cout << " - ";
            if (project->is_closed()) {
                project->get_end_date().print();
            }
        }
    }
    if (!has_projects) {
        std::cout << " None";
    }
    std::cout << std::endl;
}

void Company::print_active_staff(Params)
{
    std::set<std::string> active_employees;

    // Iterate through all projects to find active employees
    for (const auto& project_pair : projects_) {
        const Project* project = project_pair.second;
        const std::set<std::string>& project_employees = project->get_assigned_employees();

        // Insert all employee IDs from this project into the active employees set
        active_employees.insert(project_employees.begin(), project_employees.end());
    }

    // Check if there are active employees
    if (active_employees.empty()) {
        std::cout << "None" << std::endl;
    } else {
        // Print out all active employees' IDs
        for (const auto& employee_id : active_employees) {
            std::cout << employee_id << std::endl;
        }
    }
}

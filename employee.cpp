#include "employee.hh"
#include <iostream>
#include <map>

Employee::Employee() : is_active_(true)
{
}

Employee::Employee(const std::string &id):
    id_(id), is_active_(true)
{
    date_of_birth_ = Date();
}

Employee::Employee(const std::string& id, const std::string& date_of_birth):
    id_(id), date_of_birth_(date_of_birth), is_active_(true)
{
}

Employee::~Employee()
{
    //std::cout << "Employee " << id_ << " destructed." << std::endl;
}

std::string Employee::get_id() const
{
    return id_;
}

void Employee::add_skill(const std::string& skill)
{
    skills_.insert(skill);
}

bool Employee::has_skill(const std::string &skill) const
{
    for( std::string skill_item : skills_ )
    {
        if( skill_item == skill )
        {
            return true;
        }
    }
    return false;
}

void Employee::print_id(const std::string& pre_text) const
{
    std::cout << pre_text << id_;
    //date_of_birth_.print();
    //std::cout << std::endl;
}

void Employee::print_skills() const
{
    std::cout << "Skills: ";
    if( skills_.empty() )
    {
        std::cout << "None" << std::endl;
        return;
    }

    std::set<std::string>::const_iterator iter = skills_.begin();
    std::cout << *iter; // Printing the first one
    ++iter;
    while( iter != skills_.end() )
    {
        std::cout << ", " << *iter; // Printing the rest
        ++iter;
    }
    std::cout << std::endl;
}

bool Employee::operator<(const Employee &rhs) const
{
    return id_ < rhs.id_;
}

bool Employee::meets_requirements(const std::set<std::string>& required_skills) const {
    for (const auto& skill : required_skills) {
        if (skills_.count(skill) > 0) {
            return true;  // Employee has this skill, thus meets the requirement
        }
    }
    return false;  // Employee does not meet the requirements
}

const std::set<std::string>& Employee::get_skills() const {
    return skills_;
}

void Employee::activate()
{
    is_active_ = true;
}

void Employee::deactivate()
{
    is_active_ = false;
}

bool Employee::is_active() const
{
    return is_active_;
}

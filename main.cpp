/*
 * IT Company Management System
 * ------------------------------------
 * This program simulates an IT company management system, allowing the creation,
 * modification, and display of projects and employee information. It supports
 * various operations such as recruiting new employees, assigning them to projects,
 * advancing project dates, and handling employee resignations.
 *
 * Key Features:
 * - Create and manage projects with unique identifiers.
 * - Add requirements (skills) to projects.
 * - Recruit employees with unique identifiers and skills.
 * - Assign employees to projects based on their skills.
 * - Advance the current date, affecting project timelines.
 * - Print detailed information about projects and employees.
 * - Handle employees leaving the company.
 * - Maintain a history of all employees and projects, even after completion or resignation.
 *
 * The system is driven by a command-line interface (CLI) that accepts commands for
 * various operations. The CLI interacts with the `Company` class, which acts as the
 * central hub for managing `Employee` and `Project` objects. Each `Employee` has a set
 * of skills, and each `Project` has a set of skill requirements and assigned employees.
 *
 * The system ensures that employees can only be assigned to projects if their skills
 * match the project requirements. It also provides detailed reporting capabilities,
 * listing all projects and employees in a structured format, showing their attributes
 * and current status within the company.
 *
 * The program is designed to be modular and extensible, with clear separation between
 * the CLI, data management, and the underlying business logic.
 *
 * Ohjelman kirjoittaja
 * Nimi: Leevi Alanen
 * Opiskelijanumero: 151123292
 * Käyttäjätunnus: ccleal ( Git-repositorion hakemistonimi. )
 * E-Mail: leevi.alanen@tuni.fi
 *
 **/

#include "cli.hh"
#include "company.hh"

const std::string PROMPT = "IT> ";

int main()
{
    Company* company = new Company();
    Cli cli(company, PROMPT);
    while( cli.exec() ){}

    delete company;
    return EXIT_SUCCESS;
}

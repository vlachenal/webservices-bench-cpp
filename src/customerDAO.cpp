/*
 * Copyright © 2017 Vincent Lachenal
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "customerDAO.hpp"

#include <sstream>
#include <optional>

#include "sql/resultSet.hpp"
#include "sql/types/date.hpp"
#include "uuid.hpp"
#include "resource/resource.hpp"
#include <iostream>


// SQL requests +
const static std::string REQ_LIST_ALL("SELECT id,first_name,last_name FROM Customer");
const static std::string REQ_GET_CUST("SELECT id,first_name,last_name,birth_date,email FROM Customer WHERE id = ?");
const static std::string REQ_GET_CUST_ADDR("SELECT line1,line2,line3,line4,line5,line6,zip_code,city,country FROM address WHERE customer_id = ?");
const static std::string REQ_GET_CUST_PHONES("SELECT phone_type,number FROM phone WHERE customer_id = ?");
const static std::string REQ_DELETE_ALL("DELETE FROM Customer");
const static std::string ADD_CUSTOMER("INSERT INTO customer (id,first_name,last_name,birth_date,email) VALUES (?,?,?,?,?)");
const static std::string ADD_ADDRESS("INSERT INTO address (customer_id,line1,line2,line3,line4,line5,line6,zip_code,city,country) VALUES (?,?,?,?,?,?,?,?,?,?)");
const static std::string ADD_PHONE("INSERT INTO phone (customer_id,phone_type,number) VALUES (?,?,?)");
// SQL requests -


// Constructors +
CustomerDAO::CustomerDAO(): vacuumReqs(), pool(anch::sql::SqlConnectionFactory::getInstance().getPool("apibenchmark")) {
  const anch::resource::Resource& res = anch::resource::Resource::getResource("application.conf");
  std::string requests;
  bool found = res.getParameter(requests, "customer.vacuum");
  if(found) {
    std::istringstream iss(requests);
    std::string buffer;
    while(std::getline(iss, buffer, iss.widen(';'))) {
      vacuumReqs.push_back(buffer);
    }
  }
  vacuumReqs.shrink_to_fit();
}
// Constructors -


// Destructor +
CustomerDAO::~CustomerDAO() {
  // Nothing to do
}
// Destructor -


// Methods +
std::vector<CustomerDTO>
CustomerDAO::listCustomers() {
  std::vector<CustomerDTO> customers;
  pool.borrowResource().get().queryMapRow(REQ_LIST_ALL, [&customers](anch::sql::ResultSet& resSet) {
      CustomerDTO cust;
      std::string id;
      resSet.get<std::string>(0, id);
      cust.id = anch::Uuid(id);
      resSet.get<std::string>(1, cust.firstName);
      resSet.get<std::string>(2, cust.lastName);
      customers.push_back(cust);
    });
  return customers;
}

/*!
 * Get address line from result set and set it into DTO if not NULL
 *
 * \param idx the result set index
 * \param lines the address lines
 */
static void setLine(uint32_t idx, std::vector<std::string> lines, anch::sql::ResultSet& resSet) {
  std::optional<std::string> line = resSet.get<std::string>(1);
  if(line) {
    lines.push_back(line.value());
  }
}

CustomerDTO
CustomerDAO::getDetails(const std::string& uuid) {
  auto res = pool.borrowResource();
  CustomerDTO customer;
  res.get().queryMapRow(REQ_GET_CUST, [&customer](anch::sql::ResultSet& resSet) {
      std::string id;
      resSet.get<std::string>(0, id);
      customer.id = anch::Uuid(id);
      resSet.get<std::string>(1, customer.firstName);
      resSet.get<std::string>(2, customer.lastName);
      anch::sql::Date birthDate;
      resSet.get<anch::sql::Date>(3, birthDate);
      customer.birthDate = birthDate;
      resSet.get<std::string>(4, customer.email);
    }, uuid);
  res.get().queryMapRow(REQ_GET_CUST_ADDR, [&customer](anch::sql::ResultSet& resSet) {
      setLine(0, customer.address.lines, resSet);
      setLine(1, customer.address.lines, resSet);
      setLine(2, customer.address.lines, resSet);
      setLine(3, customer.address.lines, resSet);
      setLine(4, customer.address.lines, resSet);
      setLine(5, customer.address.lines, resSet);
      resSet.get<std::string>(6, customer.address.zipCode);
      resSet.get<std::string>(7, customer.address.city);
      resSet.get<std::string>(8, customer.address.country);
    }, uuid);
  res.get().queryMapRow(REQ_GET_CUST_PHONES, [&customer](anch::sql::ResultSet& resSet) {
      PhoneDTO phone;
      std::optional<int> type = resSet.get<int>(0);
      phone.type = static_cast<PhoneTypeDTO>(type.value());
      resSet.get<std::string>(1, phone.number);
      customer.phones.push_back(phone);
    }, uuid);
  return customer;
}

/*!
 * Get line if set
 *
 * \param idx the line index
 * \param lines the lines
 *
 * \return the line pointer if it exists, \code NULL otherwise
 */
static const std::string getLine(uint32_t idx, const std::vector<std::string> lines) {
  return (idx >= lines.size()) ? std::string() : lines[idx];
}

std::string
CustomerDAO::create(const CustomerDTO& customer) {
  anch::Uuid uuid;
  anch::Uuid::generateUuid(uuid);
  std::string strUuid = uuid.toString();
  auto res = pool.borrowResource();
  res.get().begin();
  try {
    res.get().update(ADD_CUSTOMER, strUuid, customer.firstName, customer.lastName, anch::sql::Date(customer.birthDate), customer.email);
    res.get().update(ADD_ADDRESS,
		     strUuid,
		     getLine(0, customer.address.lines),
		     getLine(1, customer.address.lines),
		     getLine(2, customer.address.lines),
		     getLine(3, customer.address.lines),
		     getLine(4, customer.address.lines),
		     getLine(5, customer.address.lines),
		     customer.address.zipCode,
		     customer.address.city,
		     customer.address.country);
    res.get().batchUpdate<PhoneDTO>(ADD_PHONE, [strUuid](anch::sql::PreparedStatement& stmt, const PhoneDTO& phone) {
	stmt.set(1, strUuid);
	stmt.set(2, static_cast<int>(phone.type));
	stmt.set(3, phone.number);
      }, customer.phones);
  } catch(...) {
    res.get().rollback();
    throw;
  }
  res.get().commit();
  return strUuid;
}

void
CustomerDAO::deleteAll() {
  auto res = pool.borrowResource();
  res.get().update(REQ_DELETE_ALL);
  for(auto iter = vacuumReqs.cbegin() ; iter != vacuumReqs.cend() ; ++iter) {
    res.get().update(*iter);
  }
}
// Methods -

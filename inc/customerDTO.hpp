/*
 * Copyright © 2017 Vincent Lachenal
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef CUSTOMER_DTO_HPP
#define CUSTOMER_DTO_HPP

#include <vector>
#include <string>

#include "date/date.hpp"
#include "uuid.hpp"


enum class PhoneTypeDTO {
  LANDLINE = 1,
  MOBILE = 2
};

struct PhoneDTO {
  PhoneTypeDTO type;
  std::string number;
};

struct AddressDTO {
  std::vector<std::string> lines;
  std::string zipCode;
  std::string city;
  std::string country;
};

struct CustomerDTO {
  anch::Uuid id;
  std::string firstName;
  std::string lastName;
  anch::date::Date birthDate;
  std::string email;
  AddressDTO address;
  std::vector<PhoneDTO> phones;
};

#endif // CUSTOMER_DTO_HPP

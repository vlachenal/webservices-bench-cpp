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
  PhoneDTO(): type(PhoneTypeDTO::MOBILE), number() {}
  PhoneDTO(const PhoneDTO& other): type(other.type),
				   number(other.number) {}
  PhoneDTO(PhoneDTO&& other): type(std::move(other.type)),
			      number(std::move(other.number)) {}
};

struct AddressDTO {
  std::vector<std::string> lines;
  std::string zipCode;
  std::string city;
  std::string country;
  AddressDTO(): lines(), zipCode(), city(), country() {}
  AddressDTO(const AddressDTO& other): lines(other.lines),
				       zipCode(other.zipCode),
				       city(other.city),
				       country(other.country) {}
  AddressDTO(AddressDTO&& other): lines(std::move(other.lines)),
				  zipCode(std::move(other.zipCode)),
				  city(std::move(other.city)),
				  country(std::move(other.country)) {}
};

struct CustomerDTO {
  anch::UUID id;
  std::string firstName;
  std::string lastName;
  anch::date::Date birthDate;
  std::string email;
  AddressDTO address;
  std::vector<PhoneDTO> phones;
  CustomerDTO(): id(), firstName(), lastName(), birthDate(), email(), address(), phones() {}
  CustomerDTO(const CustomerDTO& other): id(other.id),
					 firstName(other.firstName),
					 lastName(other.lastName),
					 birthDate(other.birthDate),
					 email(other.email),
					 address(other.address),
					 phones(other.phones) {}
  CustomerDTO(CustomerDTO&& other): id(other.id),
				    firstName(std::move(other.firstName)),
				    lastName(std::move(other.lastName)),
				    birthDate(std::move(other.birthDate)),
				    email(std::move(other.email)),
				    address(std::move(other.address)),
				    phones(std::move(other.phones)) {}
};

#endif // CUSTOMER_DTO_HPP

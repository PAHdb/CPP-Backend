#ifndef PAHDB_H_
#define PAHDB_H_

#include "Exception.h"
#include "PAHGeometry.h"

#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>

sql_create_18(sql_properties, 1, 0, mysqlpp::sql_int_unsigned, uid,
              mysqlpp::sql_int_unsigned, n_h, mysqlpp::sql_int_unsigned, n_c,
              mysqlpp::sql_int_unsigned, n_n, mysqlpp::sql_int_unsigned, n_o,
              mysqlpp::sql_int_unsigned, n_mg, mysqlpp::sql_int_unsigned, n_si,
              mysqlpp::sql_int_unsigned, n_fe, mysqlpp::sql_int, charge,
              mysqlpp::sql_int_unsigned, n_solo, mysqlpp::sql_int_unsigned,
              n_duo, mysqlpp::sql_int_unsigned, n_trio,
              mysqlpp::sql_int_unsigned, n_quartet, mysqlpp::sql_int_unsigned,
              n_quintet, mysqlpp::sql_int_unsigned, n_ch,
              mysqlpp::sql_int_unsigned, n_ch2, mysqlpp::sql_int_unsigned,
              n_ch3, mysqlpp::sql_int_unsigned, n_chx);

#include <unistd.h>

#include <string>
#include <string_view>
#include <utility>
#include <vector>

class PAHdb {
private:
  const char *pre[3] = {"", "exp_", "anharmonic_"};

public:
  enum class Database : char { Theory, Experiment, Anharmonic };

  void connect(std::string_view database, const std::string_view host,
               std::string_view username, const std::string_view password,
               int port, bool compress, int timeout,
               const std::string_view socket);

  void setTable(Database table);

  void setProgress(double progress);

  void setError(const char *error);

  std::vector<std::vector<std::pair<double, double>>>
  getTransitionsFromUIDsAndVersion(const std::vector<int> &uids, int version);

  std::vector<std::vector<std::pair<double, double>>>
  getTransitionsFromUIDAndVersion(int uid, int version);

  std::vector<std::vector<std::pair<double, double>>>
  getTransitionsFromVersion(int version, std::vector<int> &ids);

  std::vector<std::vector<std::pair<double, double>>>
  getTransitionsFromIds(const std::vector<int> &ids);

  std::vector<std::vector<std::pair<double, double>>>
  getTransitionsFromId(int id);

  std::vector<std::vector<std::pair<double, double>>>
  getExperimentalAndTheoreticalTransitionsFromId(int id);

  std::vector<PAHGeometry> getGeometriesFromIds(const std::vector<int> &ids);

  std::vector<sql_properties>
  getPropertiesByUIDsAndVersion(const std::vector<int> &uids, int version);

  std::vector<sql_properties> getPropertiesByIDs(const std::vector<int> &ids);

  std::vector<std::string> getFormulaeFromIds(const std::vector<int> &ids);

private:
  mysqlpp::Connection _connection;

  Database _table;
};

inline void PAHdb::setTable(Database table) { _table = table; }

inline std::vector<std::vector<std::pair<double, double>>>
PAHdb::getTransitionsFromId(int id) {
  return (getTransitionsFromIds(std::vector<int>(1, id)));
}

inline std::vector<std::vector<std::pair<double, double>>>
PAHdb::getTransitionsFromUIDAndVersion(int uid, int version) {
  return (getTransitionsFromUIDsAndVersion(std::vector<int>(1, uid), version));
}

#endif /* PAHDB_H_ */

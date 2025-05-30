#include "PAHdb.h"

void PAHdb::connect(std::string_view database = "", std::string_view host = "",
                    std::string_view username = "",
                    std::string_view password = "", int port = 0,
                    bool compress = false, int timeout = 0,
                    std::string_view socket = "") {

  try {

    _connection.connect(database.data(), socket.data(), username.data(),
                        password.data(), port);
  } catch (const mysqlpp::ConnectionFailed &e) {

    throw(Exception(e.what()));
  }
}

std::vector<std::vector<std::pair<double, double>>>
PAHdb::getTransitionsFromUIDsAndVersion(const std::vector<int> &uids,
                                        int version) {

  mysqlpp::Query query = _connection.query();

  mysqlpp::StoreQueryResult result;

  mysqlpp::Row row;

  std::vector<std::vector<std::pair<double, double>>> vector;

  std::pair<double, double> transition;

  try {

    for (const auto &uid : uids) {

      query.reset();

      query << "SELECT frequency, intensity FROM "
            << pre[static_cast<int>(_table)]
            << "transitions WHERE specie_id=(SELECT id FROM "
            << pre[static_cast<int>(_table)]
            << "species AS s1 WHERE status & 2 = 0 AND update_number=(SELECT "
               "MAX(update_number) FROM  "
            << pre[static_cast<int>(_table)]
            << "species AS s2 WHERE update_number<=" << version
            << " AND s1.uid=s2.uid) AND uid=" << uid << ")";

      result = query.store();

      std::vector<std::pair<double, double>> transitions;

      try {

        for (const auto &row : result) {

          transition.first = row["frequency"];

          transition.second = row["intensity"];

          transitions.push_back(transition);
        }
      } catch (mysqlpp::BadFieldName &e) {

        throw(Exception(e.what()));
      }

      vector.push_back(transitions);
    }
  } catch (const mysqlpp::BadQuery &e) {

    throw(Exception(e.what()));
  }

  return (vector);
}

std::vector<std::vector<std::pair<double, double>>>
PAHdb::getTransitionsFromVersion(int version, std::vector<int> &uids) {

  uids.clear();

  mysqlpp::Query query = _connection.query();

  mysqlpp::StoreQueryResult result;

  mysqlpp::Row row;

  uids.clear();

  try {

    query.reset();

    query << "SELECT uid FROM " << pre[static_cast<int>(_table)]
          << "species AS s1 WHERE status & 2 = 0 AND update_number=(SELECT "
             "MAX(update_number) FROM "
          << pre[static_cast<int>(_table)]
          << "species AS s2 WHERE update_number <=" << version
          << " AND s1.uid=s2.uid) ORDER BY uid";

    result = query.store();

    try {

      for (const auto &row : result) {

        uids.push_back(row["uid"]);
      }
    } catch (mysqlpp::BadFieldName &e) {

      throw(Exception(e.what()));
    }
  } catch (const mysqlpp::BadQuery &e) {

    throw(Exception(e.what()));
  }

  return (PAHdb::getTransitionsFromUIDsAndVersion(uids, version));
}

std::vector<std::vector<std::pair<double, double>>>
PAHdb::getTransitionsFromIds(const std::vector<int> &ids) {

  mysqlpp::Query query = _connection.query();

  mysqlpp::StoreQueryResult result;

  mysqlpp::Row row;

  std::vector<std::vector<std::pair<double, double>>> vector;

  std::pair<double, double> transition;

  try {

    for (const auto &id : ids) {

      query.reset();

      query << "SELECT frequency, intensity FROM "
            << pre[static_cast<int>(_table)]
            << "transitions WHERE specie_id=" << id;

      result = query.store();

      std::vector<std::pair<double, double>> transitions;

      try {

        for (const auto &row : result) {

          transition.first = row["frequency"];

          transition.second = row["intensity"];

          transitions.push_back(transition);
        }
      } catch (mysqlpp::BadFieldName &e) {

        throw(Exception(e.what()));
      }

      vector.push_back(transitions);
    }
  } catch (const mysqlpp::BadQuery &e) {

    throw(Exception(e.what()));
  }

  return (vector);
}

std::vector<std::vector<std::pair<double, double>>>
PAHdb::getExperimentalAndTheoreticalTransitionsFromId(int id) {

  mysqlpp::Query query = _connection.query();

  mysqlpp::StoreQueryResult result;

  mysqlpp::Row row;

  std::vector<std::vector<std::pair<double, double>>> vector;

  std::vector<std::pair<double, double>> transitions;

  std::pair<double, double> transition;

  try {

    query.reset();

    switch (_table) {

    case PAHdb::Database::Experiment:

      query << "SELECT frequency, intensity FROM "
            << "exp_transitions WHERE specie_id=" << id;

      break;

    case PAHdb::Database::Anharmonic:
    case PAHdb::Database::Theory:
    default:

      query
          << "SELECT frequency, intensity FROM "
          << "exp_transitions JOIN exp_species ON "
          << "exp_transitions.specie_id=exp_species.id "
          << " WHERE uid=(SELECT uid FROM " << pre[static_cast<int>(_table)]
          << "species WHERE id=" << id
          << ") AND update_number=(SELECT MAX(update_number) FROM exp_species "
          << "WHERE status & 2 = 0 AND uid=(SELECT uid FROM "
          << pre[static_cast<int>(_table)] << "species WHERE id=" << id << "))";
    };

    result = query.store();

    try {

      for (const auto &row : result) {

        transition.first = row["frequency"];

        transition.second = row["intensity"];

        transitions.push_back(transition);
      }
    } catch (mysqlpp::BadFieldName &e) {

      throw(Exception(e.what()));
    }

    vector.push_back(transitions);

    query.reset();

    switch (_table) {
    case PAHdb::Database::Theory:

      query << "SELECT frequency, intensity FROM "
            << "transitions WHERE specie_id=" << id;

      break;

    case PAHdb::Database::Anharmonic:
    case PAHdb::Database::Experiment:
    default:

      query << "SELECT frequency, intensity FROM "
            << "transitions JOIN species ON "
            << "transitions.specie_id=species.id "
            << " WHERE uid=(SELECT uid FROM " << pre[static_cast<int>(_table)]
            << "species WHERE id=" << id
            << ") AND update_number=(SELECT MAX(update_number) FROM species "
            << "WHERE status & 2 = 0 AND uid=(SELECT uid FROM "
            << pre[static_cast<int>(_table)] << "species WHERE id=" << id
            << "))";
    };

    result = query.store();

    transitions.clear();

    try {

      for (const auto &row : result) {

        transition.first = row["frequency"];

        transition.second = row["intensity"];

        transitions.push_back(transition);
      }

    } catch (mysqlpp::BadFieldName &e) {

      throw(Exception(e.what()));
    }

    vector.push_back(transitions);

    query.reset();

    switch (_table) {

    case PAHdb::Database::Anharmonic:

      query << "SELECT frequency, intensity FROM "
            << "anharmonic_transitions WHERE specie_id=" << id;

      break;

    case PAHdb::Database::Theory:
    case PAHdb::Database::Experiment:
    default:

      query << "SELECT frequency, intensity FROM "
            << "anharmonic_transitions JOIN anharmonic_species ON "
            << "anharmonic_transitions.specie_id=anharmonic_species.id "
            << " WHERE uid=(SELECT uid FROM " << pre[static_cast<int>(_table)]
            << "species WHERE id=" << id
            << ") AND update_number=(SELECT MAX(update_number) FROM "
               "anharmonic_species "
            << "WHERE status & 2 = 0 AND uid=(SELECT uid FROM "
            << pre[static_cast<int>(_table)] << "species WHERE id=" << id
            << "))";
    };

    result = query.store();

    transitions.clear();

    try {

      for (const auto &row : result) {

        transition.first = row["frequency"];

        transition.second = row["intensity"];

        transitions.push_back(transition);
      }
    } catch (mysqlpp::BadFieldName &e) {

      throw(Exception(e.what()));
    }

    vector.push_back(transitions);

  } catch (const mysqlpp::BadQuery &e) {

    throw(Exception(e.what()));
  }

  return (vector);
}

std::vector<PAHGeometry>
PAHdb::getGeometriesFromIds(const std::vector<int> &ids) {

  mysqlpp::Query query = _connection.query();

  mysqlpp::StoreQueryResult result;

  mysqlpp::Row row;

  std::vector<PAHGeometry> vector;

  try {

    for (const auto &id : ids) {

      query.reset();

      switch (_table) {

      case PAHdb::Database::Experiment:

        query << "SELECT x, y, z, type FROM "
              << pre[static_cast<int>(PAHdb::Database::Theory)]
              << "geometries JOIN species ON "
              << "geometries.specie_id=species.id JOIN exp_species ON "
              << "species.id=exp_species.theoretical_id WHERE exp_species.id="
              << id;

        break;

      case PAHdb::Database::Anharmonic:
      case PAHdb::Database::Theory:
      default:
        query << "SELECT * FROM " << pre[static_cast<int>(_table)]
              << "geometries WHERE specie_id=" << id << " ORDER BY position";

        break;
      };

      result = query.store();

      std::vector<Atom> atoms;

      try {

        for (const auto &row : result) {

          atoms.push_back(Atom(row["x"], row["y"], row["z"], row["type"]));
        }
      } catch (mysqlpp::BadFieldName &e) {

        throw(Exception(e.what()));
      }

      vector.push_back(PAHGeometry(atoms));
    }
  } catch (const mysqlpp::BadQuery &e) {

    throw(Exception(e.what()));
  }

  return (vector);
}

std::vector<std::string>
PAHdb::getFormulaeFromIds(const std::vector<int> &ids) {

  mysqlpp::Query query = _connection.query();

  mysqlpp::StoreQueryResult result;

  mysqlpp::Row row;

  std::vector<std::string> vector;

  try {

    for (const auto &id : ids) {

      query.reset();

      switch (_table) {

      case PAHdb::Database::Experiment:

        query << "SELECT formula, species.update_number FROM species JOIN "
              << "exp_species ON species.uid=exp_species.uid WHERE "
              << "exp_species.id=" << id
              << " ORDER BY species.update_number DESC LIMIT 1";

        break;

      case PAHdb::Database::Anharmonic:
      case PAHdb::Database::Theory:
      default:

        query << "SELECT formula FROM " << pre[static_cast<int>(_table)]
              << "species WHERE id=" << id;
        break;
      };

      result = query.store();

      row = *result.begin();

      try {

        vector.push_back(static_cast<std::string>(row["formula"]));

      } catch (mysqlpp::BadFieldName &e) {

        throw(Exception(e.what()));
      }
    }
  } catch (const mysqlpp::BadQuery &e) {

    throw(Exception(e.what()));
  }

  return (vector);
}

std::vector<sql_properties>
PAHdb::getPropertiesByUIDsAndVersion(const std::vector<int> &uids,
                                     int version) {

  std::vector<sql_properties> properties;

  mysqlpp::Query query = _connection.query();

  try {

    query.reset();

    query << "SELECT uid, n_h, n_c, n_n, n_o, n_mg, n_si, n_fe, charge, "
             "n_solo, n_duo, n_trio, n_quartet, n_quintet, n_ch2, n_chx FROM "
          << pre[static_cast<int>(_table)]
          << "species AS s1 WHERE status & 2 = 0 AND update_number=(SELECT "
             "MAX(update_number) FROM "
          << pre[static_cast<int>(_table)]
          << "species AS s2 WHERE update_number<=" << version
          << " AND s1.uid=s2.uid) AND uid IN (" << uids.front();

    std::for_each(std::next(uids.cbegin(), 1), uids.cend(),
                  [&](auto &uid) { query << "," << uid; });

    query << ") ORDER BY FIELD(uid";

    for (const auto &uid : uids) {

      query << "," << uid;
    }

    query << ")";

    query.storein(properties);

  } catch (const mysqlpp::BadQuery &e) {

    throw(Exception(e.what()));
  }

  return (properties);
}

std::vector<sql_properties>
PAHdb::getPropertiesByIDs(const std::vector<int> &ids) {

  std::vector<sql_properties> properties;

  mysqlpp::Query query = _connection.query();

  try {

    query.reset();

    switch (_table) {

    case PAHdb::Database::Experiment:

      query << "SELECT t1.uid, t2.n_h, t2.n_c, t2.n_n, t2.n_o, t2.n_mg, "
            << "t2.n_si, t2.n_fe, t2.charge, t2.n_solo, t2.n_duo, t2.n_trio, "
            << "t2.n_quartet, t2.n_quintet, t2.n_ch2, t2.n_chx FROM (SELECT * "
            << "FROM " << pre[static_cast<int>(_table)]
            << "species AS s WHERE status & 2 = 0 AND id IN (" << ids.front();

      break;

    case PAHdb::Database::Theory:
    case PAHdb::Database::Anharmonic:
    default:

      query << "SELECT uid, n_h, n_c, n_n, n_o, n_mg, n_si, n_fe, charge, "
            << "n_solo, n_duo, n_trio, n_quartet, n_quintet, n_ch2, n_chx FROM "
            << pre[static_cast<int>(_table)] << "species WHERE id IN ("
            << ids.front();
    }

    std::for_each(std::next(ids.cbegin(), 1), ids.cend(),
                  [&](auto &id) { query << "," << id; });

    query << ')';

    if (_table == PAHdb::Database::Experiment) {

      query << ") AS t1, (SELECT * FROM species AS s1 WHERE status & 2 = 0 "
               "AND update_number = (SELECT MAX(update_number) FROM species AS "
               "s3 WHERE status & 2 = 0 AND s1.uid = s3.uid)) AS t2 WHERE "
               "t1.uid = t2.uid";
    }

    query << " ORDER BY FIELD("
          << (_table == PAHdb::Database::Experiment ? "t1." : "") << "id";

    for (const auto &id : ids) {

      query << "," << id;
    }

    query << ")";

    query.storein(properties);

  } catch (const mysqlpp::BadQuery &e) {

    throw(Exception(e.what()));
  }

  return (properties);
}

void PAHdb::setProgress(double progress) {

  if (progress > 100 || progress < 0) {

    throw(Exception("progress can only be between 0 - 100"));
  }

  mysqlpp::Query query = _connection.query();

  mysqlpp::StoreQueryResult result;

  try {

    query << "UPDATE tasks SET ret_val = " << 100.0 - progress
          << " WHERE pid = " << getpid();

    result = query.store();
  } catch (const mysqlpp::BadQuery &e) {

    throw(Exception(e.what()));
  }
}

void PAHdb::setError(const char *error) {

  mysqlpp::Query query = _connection.query();

  mysqlpp::StoreQueryResult result;

  try {

    query << "UPDATE tasks SET error = '" << error
          << "' WHERE pid = " << getpid();

    result = query.store();
  } catch (const mysqlpp::BadQuery &e) {

    throw(Exception(e.what()));
  }
}

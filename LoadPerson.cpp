
enum class DBAllowExceptions {
    ALLOW,
    DENY,
};

struct LoadPersonsParams {
    string_view db_name;
    int db_connection_timeout;
    DBAllowExceptions db_allow_exceptions;
    DBLogLevel db_log_level;
    int min_age;
    int max_age;
    string_view name_filter;
}; 


vector<Person> LoadPersons(LoadPersonsParams& param) {
    DBConnector connector(param.db_allow_exceptions, param.db_log_level);
    DBHandler db;
    if (param.db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(param.db_name, param.db_connection_timeout);
    } else {
        db = connector.Connect(param.db_name, param.db_connection_timeout);
    }
    if (!param.db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << param.min_age << " and "s << param.max_age << " "s
              << "and Name like '%"s << db.Quote(param.name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
} 

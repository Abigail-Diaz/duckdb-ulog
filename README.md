# duckdb-ulog
A native C++ DuckDB extension for querying PX4 flight logs (`.ulg`) with SQL. It memory-maps the log and reads it straight into DuckDB's vectors, so there is no Python or CSV step in between.

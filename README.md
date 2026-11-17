# duckdb-ulog

> Status: in progress

A native C++ DuckDB extension for querying PX4 flight logs (`.ulg`) with SQL. It memory-maps the log and reads it straight into DuckDB's vectors, so there is no Python or CSV step in between.

## Why

The usual workflow is to convert a log before analyzing it: a Python tool such as pyulog's `ulog2csv` parses the `.ulg` and writes CSV files, which you then load into pandas or a spreadsheet. That works, but it has real costs:

- **Interpreted parsing.** A log holds a huge number of small messages, and a Python parser typically handles them one at a time in interpreted code.
- **A text round trip.** CSV stores every number as text. Each value is formatted into characters on the way out and parsed back into a number on the way in, which costs time and disk space, and the column types have to be guessed again.
- **Extra steps and files.** You convert first, then load, then analyze, and the CSVs are one more thing to keep in sync with the original log.
- **Filtering comes last.** The data has to be loaded into memory before you can ask a question of it.

This extension is meant to skip all of that. DuckDB reads the topic you ask for straight from the log in typed, columnar batches, and does the filtering and aggregation inside the engine.

## Potential

- **Ad-hoc analysis in plain SQL.** Filter, aggregate, and join topics without writing a preprocessing script.
- **Many flights at once.** Query a whole folder of logs in one statement instead of converting each one (planned).
- **Time-aligned joins.** Line up streams sampled at different rates, such as GPS and IMU data, with DuckDB's `ASOF JOIN`.
- **Easy export.** Write results to Parquet for notebooks, dashboards, or long-term storage.
- **Large datasets.** DuckDB can process data larger than memory, so fleet-scale analysis doesn't need a big machine.

How much faster this is than the pyulog-to-pandas route hasn't been measured yet. 

## Planned usage

```sql
LOAD ulog;

SELECT timestamp, lat, lon, alt
FROM read_ulog('flight_001.ulg', topic='vehicle_global_position')
WHERE alt > 100;
```


## Roadmap

- [ ] Parse the file header and message framing
- [ ] Read format (`F`) and subscription (`A`) messages to build a schema
- [ ] `read_ulog(path, topic=...)` scanning data (`D`) messages into vectors
- [ ] Tests against real logs, cross-checked with `pyulog`
- [ ] Benchmarks (none measured yet)

## Notes

- Spec: https://docs.px4.io/main/en/dev_log/ulog_file_format.html

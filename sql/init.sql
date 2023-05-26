CREATE TABLE device (
    id SERIAL PRIMARY KEY,
    deviceip VARCHAR(255) UNIQUE,
    sys_descr VARCHAR(255)
);

CREATE TABLE data_entry (
    id SERIAL PRIMARY KEY,
    timestamp TIMESTAMP,
    device_id INT REFERENCES device (id),
    consumption FLOAT
);

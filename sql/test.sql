INSERT INTO device (deviceip, sys_descr)
VALUES ('192.168.1.100', 'Router 1'),
       ('192.168.1.101', 'Switch 1'),
       ('192.168.1.102', 'Access Point');

INSERT INTO data_entry (timestamp, device_id, consumption)
VALUES ('2023-05-26 09:00:00', 1, 5.6),
       ('2023-05-26 09:00:00', 2, 2.3),
       ('2023-05-26 09:00:00', 3, 1.8),
       ('2023-05-26 10:00:00', 1, 6.2),
       ('2023-05-26 10:00:00', 2, 3.1),
       ('2023-05-26 10:00:00', 3, 2.5);
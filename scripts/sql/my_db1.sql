CREATE DATABASE IF NOT EXISTS MyDB1;
USE MyDB1;
DROP TABLE IF EXISTS nebel_data;
CREATE TABLE nebel_data(id INT, label VARCHAR(128));
INSERT INTO nebel_data(id, label) VALUES (1, 'NebelData Started!');

CREATE DATABASE IF NOT EXISTS MY_DB1;
USE MY_DB1;
DROP TABLE IF EXISTS nebel_message;
CREATE TABLE nebel_message( id INT NOT NULL AUTO_INCREMENT, message VARCHAR(128), PRIMARY KEY (id) );
INSERT INTO nebel_message(id, message) VALUES (0, 'NebelData Inited!');

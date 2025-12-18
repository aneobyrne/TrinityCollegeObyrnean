-- Information Management Project Code - Ane O'Byrne
-- File Context: Zoo Management SQL

-- Database creation (Only if it doesn't exist -> avoid dubplicates)
  CREATE DATABASE IF NOT EXISTS zoo_db;
  USE zoo_db;

-- Order of script re-run to be safe (delete old ones to recreate them safely)
SET FOREIGN_KEY_CHECKS = 0; -- To be able to delete tables even if linked
-- Drop tables in reverse order to delte dependent ones first, avoid collapses
DROP TABLE IF EXISTS Breeding;
DROP TABLE IF EXISTS Feeding;
DROP TABLE IF EXISTS MedicalRecord;
DROP TABLE IF EXISTS Animal;
DROP TABLE IF EXISTS Keeper;
DROP TABLE IF EXISTS Enclosure;
SET FOREIGN_KEY_CHECKS = 1; -- Turn protection rule back

-- -------------------------- TABLE CREATIONS --------------------------------------
-- 1. Create Base Tables (Regular Entities) From independent to dependet
CREATE TABLE Enclosure ( -- ENCLOSURE TABLE: Where Zoo Animals Live --
 enclosure_id INT AUTO_INCREMENT PRIMARY KEY, -- Unique ID that increments when more are added
 type         VARCHAR(50) NOT NULL, -- Type of enclosure
 capacity     INT NOT NULL CHECK (capacity >= 0), -- Max num of animals (no negative numbers)
 location     VARCHAR(100) -- Location of enclosure (where)
) ENGINE=InnoDB; -- Storage engine that supports FK and transactions

CREATE TABLE Keeper ( -- KEEPER TABLE: Zoo Workers that Take Care of Animals --
 keeper_id        INT AUTO_INCREMENT PRIMARY KEY,
 name             VARCHAR(100) NOT NULL,
 experience_level VARCHAR(30),
 supervisor_id    INT NULL, -- Another keeper who supervises this keeper
 CONSTRAINT keeper_supervisor_fk 
  FOREIGN KEY (supervisor_id) REFERENCES Keeper(keeper_id) -- Recursive Foreign Key (FK)
  ON DELETE SET NULL -- No supervisor set to keeper if deleted
  ON UPDATE CASCADE -- Update automatically if supervisor's id changes
) ENGINE=InnoDB;

CREATE TABLE Animal ( -- ANIMAL TABLE: Existing Animals at the Zoo --
 animal_id    INT AUTO_INCREMENT PRIMARY KEY,
 name         VARCHAR(100) NOT NULL,
 species      VARCHAR(100) NOT NULL,
 gender       ENUM('Female', 'Male', 'Unknown') DEFAULT 'Unknown', -- Only three options
 birth_date   DATE,
 enclosure_id INT NULL, -- Links animal to its enclosure
 CONSTRAINT animal_enclosure_fk
  FOREIGN KEY(enclosure_id) REFERENCES Enclosure(enclosure_id) -- enclosure_id in both tables should match
  ON DELETE SET NULL
  ON UPDATE CASCADE
) ENGINE=InnoDB;

CREATE TABLE MedicalRecord ( -- MEDICAL RECORD TABLE: Medical Info of each Zoo Animal --
 record_id INT AUTO_INCREMENT PRIMARY KEY,
 animal_id INT NOT NULL,
 vet_name  VARCHAR(100),
 diagnosis TEXT,
 treatment TEXT,
 CONSTRAINT medicalrecord_animal_fk
  FOREIGN KEY (animal_id) REFERENCES Animal(animal_id) -- animal_id must match in both tables
  ON DELETE CASCADE
  ON UPDATE CASCADE,
 CONSTRAINT medicalrecord_animal_unique UNIQUE (animal_id) -- 1:1 relationship
) ENGINE=InnoDB;

-- 2. Create Table for Weak Entity: Feeding
CREATE TABLE Feeding ( -- FEEDING TABLE : Feeding action of keepers to animals
 feeding_id   INT AUTO_INCREMENT PRIMARY KEY,
 animal_id    INT NOT NULL,
 keeper_id    INT NOT NULL,
 feeding_time DATETIME DEFAULT CURRENT_TIMESTAMP, -- Current Time as default
 food_type    VARCHAR(100),
 quantity     VARCHAR (50),
 CONSTRAINT feeding_animal_fk FOREIGN KEY (animal_id) REFERENCES Animal(animal_id)
  ON DELETE CASCADE
  ON UPDATE CASCADE,
 CONSTRAINT feeding_keeper_fk FOREIGN KEY (keeper_id) REFERENCES Keeper(keeper_id)
  ON DELETE CASCADE
  ON UPDATE CASCADE,
 CONSTRAINT feeding_unique UNIQUE (animal_id, keeper_id, feeding_time)
) ENGINE=InnoDB;
 
 -- 3. Create Table for Recursive Entity: Breeding
 -- ¡Note to me: For now in this table breeding can only lead to one offspring, if there's time MULTIPLE?!
CREATE TABLE Breeding ( -- BREEDING TABLE: Breeding between two animals to get anothe animal
 breeding_id   INT AUTO_INCREMENT PRIMARY KEY,
 breeding_date DATE,
 parent1_id    INT NOT NULL,
 parent2_id    INT NOT NULL, 
 offspring_id  INT NOT NULL, 
 CONSTRAINT breeding_parent1_fk FOREIGN KEY (parent1_id) REFERENCES Animal(animal_id)
  ON DELETE RESTRICT -- Cannot delete animals in this table
  ON UPDATE CASCADE,
 CONSTRAINT breeding_parent2_fk FOREIGN KEY (parent2_id) REFERENCES Animal(animal_id)
  ON DELETE RESTRICT
  ON UPDATE CASCADE,
 CONSTRAINT breeding_offspring_fk FOREIGN KEY (offspring_id) REFERENCES Animal(animal_id)
  ON DELETE RESTRICT
  ON UPDATE CASCADE
-- CONSTRAINT parent_gender_check CHECK (parent1_id <> parent2_id) -- Ensure its not the same animal
 ) ENGINE=InnoDB;
 
 -- -------------------------- ADDED RULES: TRIGGERS --------------------------------------
DELIMITER $$
 -- 1. Enclosure capacity enforcement: Before inserting an animal in an enclosure, check if capacity isnt exceeded
 CREATE TRIGGER animal_before_insert_trigger
 BEFORE INSERT ON Animal
 FOR EACH ROW
 BEGIN 
   DECLARE occ INT DEFAULT 0; -- occupant (animal)
   DECLARE cap INT DEFAULT 0; -- capacity (enclosure)
   DECLARE msg VARCHAR(255);
  IF NEW.enclosure_id IS NOT NULL THEN
   SELECT COUNT(*) INTO occ FROM Animal WHERE enclosure_id = NEW.enclosure_id;
   SELECT capacity INTO cap FROM Enclosure WHERE enclosure_id = NEW.enclosure_id;
   IF cap IS NULL THEN
    SIGNAL SQLSTATE '45000' 
    SET MESSAGE_TEXT = 'Target enclosure does not exist';
   ELSEIF occ + 1 > cap THEN 
    SET msg = CONCAT('Enclosure capacity exceeded (capacity = ', cap, ', current = ', occ, ').');
    SIGNAL SQLSTATE '45000' 
    SET MESSAGE_TEXT = msg;
   END IF;
 END IF;
END$$

CREATE TRIGGER animal_before_update_trigger -- Rule 1 also applies when updating tables
BEFORE UPDATE ON Animal
FOR EACH ROW
BEGIN 
   DECLARE occ INT DEFAULT 0;
   DECLARE cap INT DEFAULT 0;
   DECLARE msg VARCHAR(255);
  IF (NEW.enclosure_id IS NOT NULL) AND (OLD.enclosure_id <> NEW.enclosure_id) THEN
   SELECT COUNT(*) INTO occ FROM Animal WHERE enclosure_id = NEW.enclosure_id;
   SELECT capacity INTO cap FROM Enclosure WHERE enclosure_id = NEW.enclosure_id;
   IF cap IS NULL THEN
    SIGNAL SQLSTATE '45000' 
    SET MESSAGE_TEXT = 'Target enclosure does not exist';
   ELSEIF occ + 1 > cap THEN
    SET msg = CONCAT('Enclosure capacity exceeded (capacity = ', cap, ', current = ', occ, ').');
    SIGNAL SQLSTATE '45000' 
    SET MESSAGE_TEXT = msg;
   END IF;
 END IF;
END$$

-- 2. Breeding parents must be opposite genders
CREATE TRIGGER breeding_before_insert_trigger
BEFORE INSERT ON Breeding
FOR EACH ROW 
BEGIN 
 DECLARE g1 VARCHAR(10); -- gender of parent 1
 DECLARE g2 VARCHAR(10); -- gender of parent 2
 -- Parents can't be the same animal
 IF NEW.parent1_id = NEW.parent2_id THEN
  SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Breeding parents must be different animals';
 END IF;
 -- Parents have to have different genders
 SELECT gender INTO g1 FROM Animal WHERE animal_id = NEW.parent1_id;
 IF g1 IS NULL THEN
  SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Parent 1 does not exist';
 END IF;
 SELECT gender INTO g2 FROM Animal WHERE animal_id = NEW.parent2_id;
 IF g2 IS NULL THEN
  SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Parent 2 does not exist';
 END IF;
 IF g1 = 'Unknown' OR g2 = 'Unknown' THEN -- must know genders, unknown is not valid either
  SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Parents gender is Unknown';
 END IF;
 IF NOT ((g1 = 'Female' AND g2 = 'Male') OR (g1 = 'Male' AND g2 = 'Female')) THEN
  SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Parents must be opposite genders';
 END IF;
END$$

DELIMITER ;

-- -------------------------- SAMPLE DATA -------------------------------------- 
INSERT INTO Enclosure (type, capacity, location) VALUES
 ('Reptile House', 20, 'South'),
 ('Artic', 3, 'North'),
 ('Farm', 15, 'South'),
 ('Aviary', 20, 'East'),
 ('Savannah', 10, 'West'),
 ('Amazon', 2, 'East'),
 ('Temperate Forest', 20, 'West');
  
INSERT INTO Keeper (name, experience_level, supervisor_id) VALUES
 ('Bernard Goodwin', 'Senior', NULL), -- main supervisor
 ('Nancy Jones', 'Intermediate', NULL),
 ('Samuel Beckett', 'Intermediate', 1),
 ('Lucas Moret', 'Intermediate', 1),
 ('Franklin Sierra', 'Intermediate', 1),
 ('Dominic Myers', 'Intermediate', NULL),
 ('Emma Myers', 'Junjor', 2),
 ('Ane OByrne', 'Junior', 4);
   
INSERT INTO Animal (name, species, gender, birth_date, enclosure_id) VALUES
 ('Dexter', 'Komodo Dragon', 'Male', '2017-07-08', 1),
 ('Elliot', 'Crocodile', 'Male', '2020-10-06', 1),
 ('Fang', 'Crocodile', 'Female', '2022-01-24', 1),
 ('Draco', 'Boa Constrictor', 'Unknown', '2025-11-01', 1),
 ('Lykos', 'Artic Fox', 'Male', '2023-10-15', 2),
 ('Luna', 'Artic Fox', 'Female', '2023-08-29', 2),
 ('Eeyore', 'Donkey', 'Male', '2019-06-17', 3),
 ('Wooly', 'Sheep', 'Female', '2021-08-05', 3),
 ('Shaun', 'Sheep', 'Male', '2020-12-20', 3),
 ('Shia', 'Sheep', 'Female', '2025-11-08', 3),
 ('Rio', 'Toucan', 'Male', '2023-05-24', 4),
 ('Kevin', 'Eagle', 'Male', '2022-02-26', 4),
 ('Brenda', 'Macaw', 'Female', '2025-03-17', 4),
 ('Dumbo', 'Elephant', 'Male', '2022-02-11', 5),
 ('Ruby', 'Elephant', 'Female', '2018-01-01', 5),
 ('Gaston', 'Zebra', 'Female', '2018-04-22', 5),
 ('Daisy', 'Zebra', 'Male', '2020-08-28', 5),
 ('Bagheera', 'Jaguar', 'Female', '2024-10-06', 6),
 ('Nahuel', 'Jaguar', 'Male', '2024-09-19', 6),
 ('Red', 'Red Panda', 'Male', '2023-10-15', 7),
 ('Mei', 'Red Panda', 'Female', '2025-11-08', 7),
 ('Firefox', 'Red Panda', 'Female', '2022-03-18', 7),
 ('Copper', 'Red Panda', 'Male', '2024-03-20', 7),
 ('Nala', 'Deer', 'Female', '2018-10-19', 7),
 ('Faith', 'Deer', 'Female', '2020-01-27', 7),
 ('Bambi', 'Deer', 'Male', '2025-11-10', 7);
 
INSERT INTO MedicalRecord (animal_id, vet_name, diagnosis, treatment) VALUES
 (1, 'Dr. Smith', 'Dental Check', 'Scale cleaning'),
 (3, 'Dr. Smith', 'Dental Check; healthy', 'No treatment'),
 (5, 'Dr. OBrien', 'Routine Check; healthy', 'No treatment'),
 (6, 'Dr. OBrien', 'Routine Check; healthy', 'No treatment'),
 (7, 'Dr. Jhin', 'Laminitis', 'Hoof care'),
 (9, 'Dr. Jhin', 'Blood test', 'Medication'),
 (11, 'Dr. Lee', 'Feather mite', 'Topical'),
 (14, 'Dr. Rakan', 'Wound', 'Antibiotics'),
 (15, 'Dr. Rakan', 'Revision; healthy', 'No treatment'),
 (23, 'Dr. Xayah', 'Routine Check; Illness', 'Surgery'),
 (25, 'Dr. Xayah', 'Routine Check; healthy', 'No treatment');
    
INSERT INTO Feeding (animal_id, keeper_id, feeding_time, food_type) VALUES
 (1, 4, '2025-11-08 08:00:00', 'Meat'),
 (2, 4, '2025-11-08 09:00:00', 'Meat'),
 (3, 4, '2025-11-08 09:00:00', 'Meat'),
 (4, 4, '2025-11-08 20:30:00', 'Rat'),
 (5, 2, '2025-11-08 09:30:00', 'Eggs'),
 (6, 2, '2025-11-08 09:30:00', 'Eggs'),
 (7, 7, '2025-11-08 09:00:00', 'Hay'),
 (8, 7, '2025-11-08 09:30:00', 'Hay'),
 (9, 7, '2025-11-08 09:30:00', 'Hay'),
 (10, 7, '2025-11-08 09:30:00', 'Hay'),
 (11, 3, '2025-11-08 10:00:00', 'Seeds'),
 (12, 3, '2025-11-08 10:20:00', 'Meat'),
 (13, 3, '2025-11-08 11:00:00', 'Seeds'),
 (14, 5, '2025-11-08 13:00:00', 'Leaves'),
 (15, 5, '2025-11-08 13:00:00', 'Leaves'),
 (16, 5, '2025-11-08 13:15:00', 'Leaves'),
 (17, 5, '2025-11-08 13:15:00', 'Leaves'),
 (18, 6, '2025-11-08 07:00:00', 'Meat'),
 (19, 6, '2025-11-08 07:00:00', 'Meat'),
 (20, 8, '2025-11-08 11:00:00', 'Bamboo'),
 (21, 8, '2025-11-08 11:00:00', 'Bamboo'),
 (22, 8, '2025-11-08 11:00:00', 'Bamboo'),
 (23, 8, '2025-11-08 11:00:00', 'Bamboo'),
 (24, 8, '2025-11-08 12:00:00', 'Berries'),
 (25, 8, '2025-11-08 12:00:00', 'Berries'),
 (26, 8, '2025-11-08 12:00:00', 'Berries');
 
INSERT INTO Breeding (breeding_date, parent1_id, parent2_id, offspring_id) VALUES
 ('2025-11-08', 23, 22, 21),
 ('2025-11-08', 8, 9, 10);
    

-- -------------------------- EXAMPLE QUERIES -------------------------------------- 
-- 1. Select Query: Replaces IDs with actual string values
SELECT a.animal_id, a.name AS animal_name, k.keeper_id, k.name AS keeper_name, f.feeding_time, f.food_type
FROM Feeding f
INNER JOIN Animal a ON f.animal_id = a.animal_id
INNER JOIN Keeper k ON f.keeper_id = k.keeper_id
ORDER BY f.feeding_time;

-- 2. Aggregation Query : Add the count of animals in enclosures
SELECT e.enclosure_id, e.type, COUNT(a.animal_id) AS num_animals
FROM Enclosure e
LEFT JOIN Animal a ON e.enclosure_id = a.enclosure_id -- Show all animals and their enclosures (even if repeated)
GROUP BY e.enclosure_id, e.type; -- Group all animals in the same enclsoure
-- HAVING COUNT(a.animal_id) >= 1; -- We want to show even the NULL enclosures so leave this as comment (if wanted to show only after certain num of animals uncomment)
   
-- 3. Subquery (Exists): Find a specific keeper that fed a species (Track the junior keeper)
SELECT k.keeper_id, k.name
FROM Keeper k
WHERE EXISTS ( -- Finds at least one row that satisfies the subquery
 SELECT 1
 FROM Feeding f
 JOIN Animal a ON f.animal_id = a.animal_id
 WHERE f.keeper_id = k.keeper_id AND a.species = 'Red Panda' );
 
 -- 4.1 Data Modification: Inserting new animals to animal table (need to take enclosure capacity into account)
 INSERT INTO Animal (name, species, gender, birth_date, enclosure_id)
 VALUES ('Frost', 'Artic Fox', 'Female', '2025-11-10', 2); # Does have available capacity
 
 -- INSERT INTO Animal (name, species, gender, birth_date, enclosure_id) -- UNCOMMENT TO TRY OUT
 -- VALUES ('Simba', 'Jaguar', 'Male', '2025-11-10', 6); # Exceeds available capacity, we should get error
 
 -- 4.2 Data Modification: Inserting animals for breeding (check equal gender error) UNCOMMENT TO TRY OUT
 -- INSERT INTO Breeding (breeding_date, parent1_id, parent2_id, offspring_id)
 -- VALUES ('2025-10-11', 24, 25, 26);
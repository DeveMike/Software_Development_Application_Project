CREATE DATABASE  IF NOT EXISTS `bank_automat` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci */;
USE `bank_automat`;
-- MySQL dump 10.13  Distrib 8.0.40, for macos14 (arm64)
--
-- Host: 127.0.0.1    Database: bank_automat
-- ------------------------------------------------------
-- Server version	5.5.5-10.4.27-MariaDB

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `account`
--

DROP TABLE IF EXISTS `account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `account` (
  `idaccount` int(11) NOT NULL AUTO_INCREMENT,
  `idcustomer` int(11) NOT NULL,
  `type` enum('debit','credit') NOT NULL,
  `credit_balance` decimal(10,2) DEFAULT 0.00,
  `credit_limit` decimal(10,2) DEFAULT 0.00,
  `debit_balance` decimal(10,2) DEFAULT 0.00,
  `created_at` datetime DEFAULT current_timestamp(),
  PRIMARY KEY (`idaccount`),
  KEY `idcustomer` (`idcustomer`),
  CONSTRAINT `account_ibfk_1` FOREIGN KEY (`idcustomer`) REFERENCES `customer` (`idcustomer`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `account`
--

LOCK TABLES `account` WRITE;
/*!40000 ALTER TABLE `account` DISABLE KEYS */;
INSERT INTO `account` VALUES (2,1,'debit',0.00,0.00,0.00,'2025-01-19 16:37:47');
/*!40000 ALTER TABLE `account` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `card`
--

DROP TABLE IF EXISTS `card`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `card` (
  `idcard` int(11) NOT NULL AUTO_INCREMENT,
  `idcustomer` int(11) NOT NULL,
  `pin` varchar(255) NOT NULL,
  `cardtype` enum('single','dual') NOT NULL,
  `created_at` datetime DEFAULT current_timestamp(),
  `expiry_date` datetime DEFAULT NULL,
  PRIMARY KEY (`idcard`),
  KEY `idcustomer` (`idcustomer`),
  CONSTRAINT `card_ibfk_1` FOREIGN KEY (`idcustomer`) REFERENCES `customer` (`idcustomer`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `card`
--

LOCK TABLES `card` WRITE;
/*!40000 ALTER TABLE `card` DISABLE KEYS */;
/*!40000 ALTER TABLE `card` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `card_account`
--

DROP TABLE IF EXISTS `card_account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `card_account` (
  `idcard_account` int(11) NOT NULL AUTO_INCREMENT,
  `idcard` int(11) NOT NULL,
  `idaccount` int(11) NOT NULL,
  `type` enum('debit','credit') NOT NULL,
  PRIMARY KEY (`idcard_account`),
  KEY `idcard` (`idcard`),
  KEY `idaccount` (`idaccount`),
  CONSTRAINT `card_account_ibfk_1` FOREIGN KEY (`idcard`) REFERENCES `card` (`idcard`) ON DELETE CASCADE,
  CONSTRAINT `card_account_ibfk_2` FOREIGN KEY (`idaccount`) REFERENCES `account` (`idaccount`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `card_account`
--

LOCK TABLES `card_account` WRITE;
/*!40000 ALTER TABLE `card_account` DISABLE KEYS */;
/*!40000 ALTER TABLE `card_account` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `customer`
--

DROP TABLE IF EXISTS `customer`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `customer` (
  `idcustomer` int(11) NOT NULL AUTO_INCREMENT,
  `fname` varchar(45) NOT NULL,
  `lname` varchar(45) NOT NULL,
  `thumbnail` varchar(255) DEFAULT NULL,
  `status` enum('active','locked','inactive') NOT NULL,
  `created_at` datetime DEFAULT current_timestamp(),
  PRIMARY KEY (`idcustomer`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `customer`
--

LOCK TABLES `customer` WRITE;
/*!40000 ALTER TABLE `customer` DISABLE KEYS */;
INSERT INTO `customer` VALUES (1,'John','Doe',NULL,'active','2025-01-19 16:37:39');
/*!40000 ALTER TABLE `customer` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `transaction`
--

DROP TABLE IF EXISTS `transaction`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `transaction` (
  `idtransaction` int(11) NOT NULL AUTO_INCREMENT,
  `idaccount` int(11) NOT NULL,
  `type` enum('withdrawal','deposit') NOT NULL,
  `amount` decimal(10,2) NOT NULL,
  `timestamp` datetime DEFAULT current_timestamp(),
  `description` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idtransaction`),
  KEY `idaccount` (`idaccount`),
  CONSTRAINT `transaction_ibfk_1` FOREIGN KEY (`idaccount`) REFERENCES `account` (`idaccount`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=17 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `transaction`
--

LOCK TABLES `transaction` WRITE;
/*!40000 ALTER TABLE `transaction` DISABLE KEYS */;
/*!40000 ALTER TABLE `transaction` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'bank_automat'
--
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-01-20 20:14:45

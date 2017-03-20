-- MySQL dump 10.13  Distrib 5.7.9, for Win64 (x86_64)
--
-- Host: localhost    Database: db_base
-- ------------------------------------------------------
-- Server version	5.7.9-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `db_base`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `db_base` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `db_base`;

--
-- Table structure for table `tb_user`
--

DROP TABLE IF EXISTS `tb_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_user` (
  `user_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `user_phone` char(11) NOT NULL,
  `user_pwd` varchar(18) NOT NULL,
  `user_reg_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_user`
--

LOCK TABLES `tb_user` WRITE;
/*!40000 ALTER TABLE `tb_user` DISABLE KEYS */;
INSERT INTO `tb_user` VALUES (1,'13000000000','123456','2016-01-11 05:15:02'),(2,'13600000001','123456789012345678','2016-01-11 05:15:02'),(3,'13001034845','666666','2016-01-11 05:15:02'),(4,'13803905011','ii1234a56','2016-01-11 05:15:02'),(5,'13100040100','123sEE6','2016-01-11 05:15:02'),(6,'13000000000','123456','2016-01-11 05:15:02'),(7,'13330000001','newpwd','2016-01-11 05:15:02'),(8,'13000000010','6rj366','2016-01-11 05:15:02'),(9,'13800430011','ii1234a56','2016-01-11 05:15:02'),(10,'13098200100','12345EE6','2016-01-11 05:15:02'),(11,'xxxxxxxxxxx','xxxxxx','2016-01-03 07:43:39'),(12,'a','b','2016-01-03 07:43:39');
/*!40000 ALTER TABLE `tb_user` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = gbk */ ;
/*!50003 SET character_set_results = gbk */ ;
/*!50003 SET collation_connection  = gbk_chinese_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`ub`@`%`*/ /*!50003 trigger initialization after insert
		on tb_user for each row
		begin
			insert into tb_user_custom							
				values (new.user_id, new.user_phone, 0);
		end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `tb_user_custom`
--

DROP TABLE IF EXISTS `tb_user_custom`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_user_custom` (
  `user_id` int(10) unsigned NOT NULL,
  `user_nickname` varchar(12) DEFAULT NULL,
  `user_score` int(10) unsigned DEFAULT '0',
  KEY `user_id` (`user_id`),
  CONSTRAINT `tb_user_custom_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `tb_user` (`user_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_user_custom`
--

LOCK TABLES `tb_user_custom` WRITE;
/*!40000 ALTER TABLE `tb_user_custom` DISABLE KEYS */;
INSERT INTO `tb_user_custom` VALUES (1,'13000000000',1000),(2,'13600000001',891),(3,'13001034845',0),(4,'13803905011',10),(5,'13100040100',0),(6,'13000000000',0),(7,'13330000001',0),(8,'13000000010',0),(9,'13800430011',0),(10,'13098200100',0),(11,'xxxxxxxxxxx',0),(12,'a',0);
/*!40000 ALTER TABLE `tb_user_custom` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = gbk */ ;
/*!50003 SET character_set_results = gbk */ ;
/*!50003 SET collation_connection  = gbk_chinese_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`ub`@`%`*/ /*!50003 trigger update_ranking after update 
		on tb_user_custom for each row
		begin
			declare old_score int unsigned;			
			declare new_score int unsigned;
			declare new_nickname varchar(12);
			
			select user_score
				from db_extra.tb_ranking
					into old_score;
					
			select user_score, user_nickname
				from tb_user_custom 
					where user_id = old.user_id
						into new_score, new_nickname;
						
			if new_score > old_score then
				update db_extra.tb_ranking
					set user_nickname = new_nickname, user_score = new.user_score;
			end if;
		end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Current Database: `db_extra`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `db_extra` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `db_extra`;

--
-- Table structure for table `tb_ranking`
--

DROP TABLE IF EXISTS `tb_ranking`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_ranking` (
  `user_nickname` varchar(12) DEFAULT 'NULL',
  `user_score` int(10) unsigned DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_ranking`
--

LOCK TABLES `tb_ranking` WRITE;
/*!40000 ALTER TABLE `tb_ranking` DISABLE KEYS */;
INSERT INTO `tb_ranking` VALUES ('13000000000',8910);
/*!40000 ALTER TABLE `tb_ranking` ENABLE KEYS */;
UNLOCK TABLES;

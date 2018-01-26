/*
SQLyog Ultimate v8.8 
MySQL - 5.7.14 : Database - lms-inno
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`lms-inno` /*!40100 DEFAULT CHARACTER SET latin1 */;

USE `lms-inno`;

/*Table structure for table `av` */

DROP TABLE IF EXISTS `av`;

CREATE TABLE `av` (
  `av_id` int(11) NOT NULL AUTO_INCREMENT,
  `title` text,
  `authors` text,
  `keywords` text,
  `price` int(11) unsigned DEFAULT '0',
  PRIMARY KEY (`av_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `av` */

/*Table structure for table `book` */

DROP TABLE IF EXISTS `book`;

CREATE TABLE `book` (
  `book-id` int(11) NOT NULL AUTO_INCREMENT,
  `authors` text,
  `title` text,
  `publisher` text,
  `edition` text,
  `keywords` text,
  `price` int(11) unsigned DEFAULT '0',
  `best-seller` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`book-id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `book` */

/*Table structure for table `check_out` */

DROP TABLE IF EXISTS `check_out`;

CREATE TABLE `check_out` (
  `check_out_id` int(11) NOT NULL AUTO_INCREMENT,
  `copy_id` int(11) DEFAULT NULL,
  `user_id` int(11) DEFAULT NULL,
  `taken` date NOT NULL,
  `returned` date DEFAULT NULL,
  PRIMARY KEY (`check_out_id`),
  KEY `FK_check_out` (`user_id`),
  KEY `FK1_check_out` (`copy_id`),
  CONSTRAINT `FK1_check_out` FOREIGN KEY (`copy_id`) REFERENCES `copy` (`copy_id`),
  CONSTRAINT `FK_check_out` FOREIGN KEY (`user_id`) REFERENCES `user` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `check_out` */

/*Table structure for table `copy` */

DROP TABLE IF EXISTS `copy`;

CREATE TABLE `copy` (
  `copy_id` int(11) NOT NULL AUTO_INCREMENT,
  `prototype` int(11) unsigned DEFAULT NULL,
  `document_type` int(11) unsigned DEFAULT NULL,
  `checked_out` tinyint(1) DEFAULT NULL,
  `room` int(11) unsigned DEFAULT NULL,
  `level` int(11) unsigned DEFAULT NULL,
  PRIMARY KEY (`copy_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `copy` */

/*Table structure for table `journal_article` */

DROP TABLE IF EXISTS `journal_article`;

CREATE TABLE `journal_article` (
  `article_id` int(11) NOT NULL AUTO_INCREMENT,
  `title` text,
  `authors` text,
  `publisher` text,
  `editor` text,
  `journal` text,
  `keywords` text,
  `price` int(11) unsigned DEFAULT '0',
  `month` text,
  `year` int(4) unsigned DEFAULT '2000',
  PRIMARY KEY (`article_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `journal_article` */

/*Table structure for table `user` */

DROP TABLE IF EXISTS `user`;

CREATE TABLE `user` (
  `user_id` int(11) NOT NULL AUTO_INCREMENT,
  `name` text,
  `adress` text,
  `phone` text,
  `function` int(3) unsigned DEFAULT NULL,
  `username` text,
  `password` text,
  `card_number` int(11) DEFAULT NULL,
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `user` */

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

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
  `title` varchar(30) NOT NULL,
  `authors` varchar(50) NOT NULL,
  `keywords` varchar(50) NOT NULL,
  `price` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`av_id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;

/*Data for the table `av` */

insert  into `av`(`av_id`,`title`,`authors`,`keywords`,`price`) values (1,'okja','Joon-ho Bong','action adventure drama',1700),(2,'Call me by your name','Luca Guadagnino','drama romance',1850),(3,'Murder on the Orient Express ','Kenneth Branagh','crime drama mystery',2100),(4,'Dunkirk','Christopher Nolan','action drama history',1350);

/*Table structure for table `book` */

DROP TABLE IF EXISTS `book`;

CREATE TABLE `book` (
  `book-id` int(11) NOT NULL AUTO_INCREMENT,
  `authors` varchar(50) NOT NULL,
  `title` varchar(30) NOT NULL,
  `publisher` varchar(30) NOT NULL,
  `edition` varchar(30) NOT NULL,
  `keywords` varchar(50) NOT NULL,
  `price` int(11) unsigned NOT NULL DEFAULT '0',
  `best-seller` tinyint(1) NOT NULL,
  PRIMARY KEY (`book-id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;

/*Data for the table `book` */

insert  into `book`(`book-id`,`authors`,`title`,`publisher`,`edition`,`keywords`,`price`,`best-seller`) values (1,'J.R. Palacio','Wonder','Knopf Books for Young Readers','1st','children friendship school',750,1),(2,'J. D. Vance','Hillbilly Elegy','Harper','1st','nature story',1225,0),(3,'Gary Chapman ',' The Secret to Love that Lasts','Northfield Publishing','2nd','love',525,1);

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
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

/*Data for the table `check_out` */

insert  into `check_out`(`check_out_id`,`copy_id`,`user_id`,`taken`,`returned`) values (2,1,3,'2018-05-17','0000-00-00');

/*Table structure for table `copy` */

DROP TABLE IF EXISTS `copy`;

CREATE TABLE `copy` (
  `copy_id` int(11) NOT NULL AUTO_INCREMENT,
  `prototype` int(11) unsigned NOT NULL,
  `document_type` int(11) unsigned NOT NULL,
  `checked_out` tinyint(1) NOT NULL,
  `room` int(11) unsigned NOT NULL,
  `level` int(11) unsigned NOT NULL,
  PRIMARY KEY (`copy_id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

/*Data for the table `copy` */

insert  into `copy`(`copy_id`,`prototype`,`document_type`,`checked_out`,`room`,`level`) values (1,1,1,0,5,6),(2,2,3,0,5,7),(3,3,2,1,3,2);

/*Table structure for table `journal_article` */

DROP TABLE IF EXISTS `journal_article`;

CREATE TABLE `journal_article` (
  `article_id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(30) NOT NULL,
  `authors` varchar(50) NOT NULL,
  `publisher` varchar(50) NOT NULL,
  `editor` varchar(50) NOT NULL,
  `journal` varchar(30) NOT NULL,
  `keywords` varchar(50) NOT NULL,
  `price` int(11) unsigned NOT NULL DEFAULT '0',
  `month` varchar(10) NOT NULL,
  `year` int(4) unsigned NOT NULL DEFAULT '2000',
  PRIMARY KEY (`article_id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

/*Data for the table `journal_article` */

insert  into `journal_article`(`article_id`,`title`,`authors`,`publisher`,`editor`,`journal`,`keywords`,`price`,`month`,`year`) values (1,'C++','Ince Darrel','Oxford University Press 2013','Daintith, John','A Dictionary of the Internet','programming cs',1400,'june',2013),(2,'Machine learning','Wojtusiak, Janusz','Peer Reviewed Journal','Mitchel T. M.','Encyclopedia of the Sciences o','python cs',750,'january',1983),(3,'Entanglement beyond identical ','Tobias Schaetz','Peer Reviewed Journal','Gusterson Hugh','Quantum physics','atom physics',1250,'may',2015);

/*Table structure for table `user` */

DROP TABLE IF EXISTS `user`;

CREATE TABLE `user` (
  `user_id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(40) NOT NULL,
  `adress` varchar(30) NOT NULL,
  `phone` varchar(15) NOT NULL,
  `function` int(3) unsigned NOT NULL,
  `username` varchar(20) NOT NULL,
  `password` varchar(80) NOT NULL,
  `card_number` int(11) NOT NULL,
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

/*Data for the table `user` */

insert  into `user`(`user_id`,`name`,`adress`,`phone`,`function`,`username`,`password`,`card_number`) values (1,'Nikola Novarlic','Innopolis 6','065 132',1,'n.nikola','123456',1),(2,'Dragos Strugar','Innopolis 6','065 788',2,'strudra','internet',2),(3,'Luka Jankovic','Novi Sad bb','066 546',3,'jluka','password',3);

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

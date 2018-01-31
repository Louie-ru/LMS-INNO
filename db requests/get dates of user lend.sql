SELECT 
  `check_out`.`taken` 
FROM
  `user`,
  `check_out` 
WHERE `user`.`user_id` = `check_out`.`user_id` 
  AND `user`.`username` = "jluka"
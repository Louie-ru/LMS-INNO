SELECT 
  check_out_id 
FROM
  check_out 
WHERE `user_id` = 2 
  AND `item_type` = 'book' 
  AND item_id = 2 
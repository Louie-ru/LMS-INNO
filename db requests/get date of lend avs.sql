SELECT 
  taken 
FROM
  check_out,
  av 
WHERE av.`av_id` = check_out.`item_id` AND check_out.`item_type` = "av"
  AND av.`av_id` = 1
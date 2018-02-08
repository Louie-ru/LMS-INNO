SELECT 
  title 
FROM
  check_out,
  av
WHERE av.`av_id` = check_out.`item_id` 
  AND item_type = "av" 
  AND returned = "0000-00-00" 
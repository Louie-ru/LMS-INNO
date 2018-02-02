SELECT 
  title 
FROM
  check_out,
  book 
WHERE book.`book-id` = check_out.`item_id` 
  AND item_type = "book" 
  AND returned = "0000-00-00" 
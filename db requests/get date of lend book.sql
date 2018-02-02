SELECT 
  taken 
FROM
  check_out,
  book 
WHERE book.`book-id` = check_out.`item_id` AND check_out.`item_type` = "book"
  AND book.`book-id` = 1
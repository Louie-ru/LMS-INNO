SELECT 
  taken 
FROM
  check_out,
  journal_article
WHERE journal_article.`article_id` = check_out.`item_id` AND check_out.`item_type` = "journal article"
  AND journal_article.`article_id` = 1
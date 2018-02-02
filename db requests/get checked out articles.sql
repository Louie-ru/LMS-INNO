SELECT 
  title 
FROM
  check_out,
  journal_article
WHERE journal_article.`article_id` = check_out.`item_id` 
  AND item_type = "journal article" 
  AND returned = "0000-00-00" 
((nil . ((tab-width . 2)
				 (eval .
							 (progn
								 (let* ((build-root (file-name-as-directory (f-join (dir-locals-dir) "build")))
												(targets (delete ".." (delete "." (directory-files build-root))))
												(target-dirs (mapcar (lambda (d) (file-name-as-directory (f-join build-root d))) targets)))
									 (dolist (d target-dirs)
										 (let ((include-dir (file-name-as-directory (f-join d "include"))))
											 (add-c-include-path include-dir))))
								 (let ((include-dir (f-join (dir-locals-dir) "dl")))
									 (add-c-include-path include-dir))))))
 (scheme-mode . ((indent-tabs-mode . nil)))
 (c-mode . ((indent-tabs-mode . nil)
						(eval . (progn
											(add-c-build-arg "-std=c99")))))
 (c++-mode . ((indent-tabs-mode . nil)
							(eval . (progn
												(add-c-build-arg "-std=c++14"))))))

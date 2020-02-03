ALL:
	gcc bmp2mcp.c -o bmp2mcp
change:
	for i in `ls bmp`;do bmp2mcp bmp/$i > mcp/"`basename $i .bmp`.h";done
clean:
	rm -rf bmp2mcp
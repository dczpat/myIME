from pypinyin import pinyin, lazy_pinyin, Style

duoyinzi = set()
ignore = set()
tmp_pinyin = ""
cnt = 0


with open("ignore.txt", "r", encoding="gbk") as f0:
    for line in f0:
        ignore.add(line.strip())


with open("duoyinzi.txt", "r", encoding="gbk") as f1:
    for line in f1:
        duoyinzi.add(line[0])


with open("dyc_new.txt", "r", encoding="gbk") as f2:
    for line in f2.readlines():
        cnt = 0
        phrase = line.strip()
        for i, c in enumerate(phrase):
            if c in duoyinzi:
                cnt += 1
                tmp_pinyin = lazy_pinyin(phrase)[i]
        if cnt > 1:
            with open("ignore.txt", "a", encoding="gbk") as f3:
                if phrase not in ignore:
                    f3.write(phrase+"\n")
                    ignore.add(phrase)
        elif cnt == 1:
            with open("duoyinci.txt", "a", encoding="gbk") as f4:
                f4.write(phrase+" "+tmp_pinyin+"\n")
        else:
            with open("error.txt", "a", encoding="gbk") as f5:
                f5.write(phrase+"\n")

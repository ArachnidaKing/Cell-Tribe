echo 设置用户信息

git config --global user.name Victor

set/p a1=请输入邮箱：

git config --global user.email %ver%

echo 初始化目录

git init

git remote add origin https://github.com/Xciyang/Cell-Tribe.git

git add .

git commit -m "init commit"

pause
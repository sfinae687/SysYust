import os
import shutil
import sys

def process_files(directory):
    # 遍历目录下的所有文件
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith('.cpp') or file.endswith('.h'):
                file_path = os.path.join(root, file)
                backup_path = file_path + '.backup'

                # 创建备份文件
                shutil.copyfile(file_path, backup_path)

                # 读取文件内容
                with open(file_path, 'r') as f:
                    lines = f.readlines()

                # 删除前两行
                lines = lines[2:]

                # 如果文件名是 SysYLexer.cpp，在文件头添加预处理宏
                if file == 'SysYLexer.h':
                    lines.insert(0, '#define ANTLR4_USE_THREAD_LOCAL_CACHE 1\n')

                # 写回文件
                with open(file_path, 'w') as f:
                    f.writelines(lines)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("用法: python process_cpp_files.py <目录路径>")
        sys.exit(1)

    directory = sys.argv[1]

    if not os.path.isdir(directory):
        print(f"错误: {directory} 不是一个有效的目录路径")
        sys.exit(1)

    process_files(directory)
    print("处理完成。")

import json
import random
import os
import sys

# 独立词库文件（方便维护）
WORD_FILE = "/home/xuzichun/文档/Obsidian Vault/LInux/back.py/cet4_words_list.txt"

def load_cet4_words():
    """加载四级新增词汇库"""
    try:
        with open(WORD_FILE, "r", encoding="utf-8") as f:
            return [word.strip() for word in f.readlines() if word.strip()]
    except FileNotFoundError:
        print(f"❌ 词库文件未找到：{WORD_FILE}")
        sys.exit(1)
    except Exception as e:
        print(f"❌ 读取词库失败：{str(e)}")
        sys.exit(1)

def generate_cet4_words():
    """生成四级词汇列表（400个新增词汇）"""
    # 加载词库
    cet4_words = load_cet4_words()
    
    # 固定选择10个单词（去重处理）
    selected_words = list(set(random.sample(cet4_words, k=min(10, len(cet4_words)))))
    
    # 构建正确的输出格式
    output = {word: word for word in selected_words}  # 修正为键值相同的格式
    
    try:
        # 确保目标目录存在
        output_dir = os.path.dirname(WORD_FILE)
        os.makedirs(output_dir, exist_ok=True)
        
        # 保存到文件
        output_path = os.path.join(output_dir, "cet4_words.json")
        with open(output_path, "w", encoding="utf-8") as f:
            json.dump(output, f, ensure_ascii=False, indent=2)
        
        return output

    except Exception as e:
        print(f"❌ 发生错误：{str(e)}")
        sys.exit(1)

if __name__ == "__main__":
    try:
        print("🚀 开始生成单词列表...")
        result = generate_cet4_words()
        print("✅ 生成成功！文件保存在：")
        print(f"/home/xuzichun/文档/Obsidian Vault/LInux/back.py/cet4_words.json")
        print("\n生成内容示例：")
        print(json.dumps(result, ensure_ascii=False, indent=2))
    except Exception as e:
        print(f"❌ 程序运行出错：{str(e)}")
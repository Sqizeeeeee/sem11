import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import os

def setup_dark_theme():
    """Настройка темной темы для графиков"""
    plt.style.use('dark_background')
    sns.set_style("darkgrid")
    
    # Цветовая схема
    colors = {
        'standard_actual': '#FF6B6B',    # Яркий красный
        'strassen_actual': '#4ECDC4',     # Яркий бирюзовый  
        'standard_theory': '#8B0000',     # Темный красный
        'strassen_theory': '#006D6D',     # Темный бирюзовый
        'background': '#0A0A0A',          # Почти черный
        'grid': '#2A2A2A',                # Темно-серый
        'text': '#E8E8E8'                 # Светло-серый
    }
    
    plt.rcParams['figure.facecolor'] = colors['background']
    plt.rcParams['axes.facecolor'] = colors['background']
    plt.rcParams['grid.color'] = colors['grid']
    plt.rcParams['text.color'] = colors['text']
    plt.rcParams['axes.labelcolor'] = colors['text']
    plt.rcParams['xtick.color'] = colors['text']
    plt.rcParams['ytick.color'] = colors['text']
    
    return colors

def load_data():
    """Загрузка данных из CSV файлов"""
    standard_df = pd.read_csv('data/standard_results.csv')
    strassen_df = pd.read_csv('data/strassen_results.csv')
    return standard_df, strassen_df

def plot_time_comparison(standard_df, strassen_df, colors):
    """График сравнения времени выполнения"""
    plt.figure(figsize=(10, 6))
    
    plt.plot(standard_df['size'], standard_df['time_ms'], 'o-', 
             color=colors['standard_actual'], label='Standard O(n³)', 
             linewidth=3, markersize=8, markerfacecolor=colors['standard_actual'])
    plt.plot(strassen_df['size'], strassen_df['time_ms'], 's-', 
             color=colors['strassen_actual'], label='Strassen O(n²·⁸¹)', 
             linewidth=3, markersize=8, markerfacecolor=colors['strassen_actual'])
    
    plt.xlabel('Matrix Size (n x n)', fontsize=12)
    plt.ylabel('Time (ms)', fontsize=12)
    plt.title('Practical Performance: Time Comparison', fontsize=14, fontweight='bold')
    plt.legend(fontsize=11)
    plt.grid(True, alpha=0.2)
    plt.tight_layout()
    plt.savefig('graphs/time_comparison.png', dpi=300, bbox_inches='tight',
                facecolor=colors['background'])
    plt.show()

def plot_theoretical_complexity(standard_df, strassen_df, colors):
    """График теоретической сложности"""
    plt.figure(figsize=(10, 6))
    
    n = np.array(standard_df['size'])
    
    # Теоретические кривые
    theoretical_standard = n**3
    theoretical_strassen = n**2.807
    
    # Нормализуем
    theoretical_standard = theoretical_standard / theoretical_standard.max()
    theoretical_strassen = theoretical_strassen / theoretical_strassen.max()
    
    # Фактические данные (нормализованные)
    actual_standard = standard_df['time_ms'] / standard_df['time_ms'].max()
    actual_strassen = strassen_df['time_ms'] / strassen_df['time_ms'].max()
    
    # Теоретические кривые (бледные)
    plt.plot(n, theoretical_standard, '--', color=colors['standard_theory'], 
             label='Theoretical O(n³)', linewidth=2, alpha=0.6)
    plt.plot(n, theoretical_strassen, '--', color=colors['strassen_theory'], 
             label='Theoretical O(n²·⁸¹)', linewidth=2, alpha=0.6)
    
    # Фактические данные (яркие)
    plt.plot(n, actual_standard, 'o-', color=colors['standard_actual'], 
             label='Standard Actual', linewidth=3, markersize=6, alpha=0.9)
    plt.plot(n, actual_strassen, 's-', color=colors['strassen_actual'], 
             label='Strassen Actual', linewidth=3, markersize=6, alpha=0.9)
    
    plt.xlabel('Matrix Size', fontsize=12)
    plt.ylabel('Normalized Values', fontsize=12)
    plt.title('Theoretical vs Actual Complexity', fontsize=14, fontweight='bold')
    plt.legend(fontsize=11)
    plt.grid(True, alpha=0.2)
    plt.tight_layout()
    plt.savefig('graphs/theoretical_vs_actual.png', dpi=300, bbox_inches='tight',
                facecolor=colors['background'])
    plt.show()

def plot_speedup_ratio(standard_df, strassen_df, colors):
    """График ускорения/замедления"""
    plt.figure(figsize=(10, 6))
    
    ratio = standard_df['time_ms'].values / strassen_df['time_ms'].values
    
    plt.plot(standard_df['size'], ratio, '^-', color='#FFD93D', 
             linewidth=3, markersize=10, markerfacecolor='#FFD93D')
    plt.axhline(y=1, color='white', linestyle='--', alpha=0.7, linewidth=2, label='Break-even')
    
    # Заливка областей
    plt.fill_between(standard_df['size'], ratio, 1, where=(ratio > 1), 
                     color=colors['strassen_actual'], alpha=0.3, label='Strassen faster')
    plt.fill_between(standard_df['size'], ratio, 1, where=(ratio < 1), 
                     color=colors['standard_actual'], alpha=0.3, label='Standard faster')
    
    plt.xlabel('Matrix Size', fontsize=12)
    plt.ylabel('Speedup Ratio (Standard/Strassen)', fontsize=12)
    plt.title('Speedup Ratio Analysis', fontsize=14, fontweight='bold')
    plt.legend(fontsize=11)
    plt.grid(True, alpha=0.2)
    plt.tight_layout()
    plt.savefig('graphs/speedup_ratio.png', dpi=300, bbox_inches='tight',
                facecolor=colors['background'])
    plt.show()

def plot_log_scale_comparison(standard_df, strassen_df, colors):
    """График в логарифмическом масштабе"""
    plt.figure(figsize=(10, 6))
    
    plt.loglog(standard_df['size'], standard_df['time_ms'], 'o-', 
               color=colors['standard_actual'], label='Standard', 
               linewidth=3, markersize=8, markerfacecolor=colors['standard_actual'])
    plt.loglog(strassen_df['size'], strassen_df['time_ms'], 's-', 
               color=colors['strassen_actual'], label='Strassen', 
               linewidth=3, markersize=8, markerfacecolor=colors['strassen_actual'])
    
    plt.xlabel('Matrix Size (log scale)', fontsize=12)
    plt.ylabel('Time (ms, log scale)', fontsize=12)
    plt.title('Log-Log Scale Comparison', fontsize=14, fontweight='bold')
    plt.legend(fontsize=11)
    plt.grid(True, alpha=0.2)
    plt.tight_layout()
    plt.savefig('graphs/log_scale_comparison.png', dpi=300, bbox_inches='tight',
                facecolor=colors['background'])
    plt.show()

def main():
    """Основная функция"""
    colors = setup_dark_theme()
    
    # Загрузка данных
    standard_df, strassen_df = load_data()
    
    # Создание отдельных графиков
    print("Creating time comparison plot...")
    plot_time_comparison(standard_df, strassen_df, colors)
    
    print("Creating theoretical vs actual plot...")
    plot_theoretical_complexity(standard_df, strassen_df, colors)
    
    print("Creating speedup ratio plot...")
    plot_speedup_ratio(standard_df, strassen_df, colors)
    
    print("Creating log-scale plot...")
    plot_log_scale_comparison(standard_df, strassen_df, colors)
    
    print("All plots saved to graphs/ folder!")

if __name__ == "__main__":
    main()
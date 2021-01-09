# Generated by Django 3.1.4 on 2021-01-09 13:09

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('algorithms', '0012_algorithm_counted'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='algorithm',
            name='results10',
        ),
        migrations.AlterField(
            model_name='outcome',
            name='best',
            field=models.CharField(max_length=30),
        ),
        migrations.AlterField(
            model_name='outcome',
            name='median',
            field=models.CharField(max_length=30),
        ),
        migrations.AlterField(
            model_name='outcome',
            name='std',
            field=models.CharField(max_length=30),
        ),
        migrations.AlterField(
            model_name='outcome',
            name='worst',
            field=models.CharField(max_length=30),
        ),
    ]

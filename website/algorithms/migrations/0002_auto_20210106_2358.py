# Generated by Django 3.1.4 on 2021-01-06 23:58

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('algorithms', '0001_initial'),
    ]

    operations = [
        migrations.CreateModel(
            name='Benchmark',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('name', models.CharField(max_length=40)),
            ],
        ),
        migrations.AddField(
            model_name='algorithm',
            name='alg_file',
            field=models.FileField(default='no_file_attached', upload_to=''),
            preserve_default=False,
        ),
        migrations.CreateModel(
            name='Outcome',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('dimension', models.IntegerField()),
                ('algorithm', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='algorithms.algorithm')),
                ('benchmark', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='algorithms.benchmark')),
            ],
        ),
    ]